#include "MainWindow.h"

#include <qgifimage.h>

#include <QColorDialog>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QImageWriter>
#include <QMessageBox>
#include <QVariantAnimation>
#include "Buttons/DeleteButton.h"
#include "./ui_mainwindow.h"
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setUnifiedTitleAndToolBarOnMac(true);
  loadSettings();
  applySettings();

  connect(ui->openGLWidget, SIGNAL(initialized()), this,
          SLOT(SetupEObjectTreeView()));
  connect(ui->toolButton_tab1, SIGNAL(clicked()), this, SLOT(choose_file()));
  connect(ui->openGLWidget, SIGNAL(importComleted(long, long, QString)), this,
          SLOT(updateInfoLabels(long, long, QString)));
  connect(ui->DrawConfigWid, SIGNAL(DrawConfigUpdated()), this,
          SLOT(UpdateGL()));
}
void MainWindow::TranslationTest(QVector3D values) { qDebug() << values; }
MainWindow::~MainWindow() { delete ui; }
void MainWindow::closeEvent(QCloseEvent* event) {
  Q_UNUSED(event)
  saveSettings();
}
void MainWindow::saveSettings() {
  QSettings settings("settings.ini", QSettings::IniFormat);
  settings.beginGroup("Settings");
  settings.setValue("lineColor", lineColor);
  settings.setValue("pointColor", pointColor);
  settings.setValue("backColor", backColor);
  settings.setValue("lineWidth", lineWidth);
  settings.setValue("pointWidth", pointWidth);
  settings.setValue("pointTypeIndex", pointTypeIndex);
  settings.setValue("viewTypeIndex", viewTypeIndex);
  settings.setValue("fileFieldIndex", fileFieldIndex);
  settings.setValue("scale", scale);

  //  settings.setValue("cameraPos", ui->openGLWidget->camera->getPosition());
  //  settings.setValue("cameraOrient",
  //  ui->openGLWidget->camera->getOrientation());

  settings.beginWriteArray("filePaths");
  int i = 0;
  for (auto& filePath : filePaths) {
    settings.setArrayIndex(i++);
    settings.setValue("path", filePath);
  }
  settings.endArray();
  settings.endGroup();
}

void MainWindow::loadSettings() {
  if (QFile::exists("settings.ini")) {
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Settings");
    int size = settings.beginReadArray("filePaths");
    for (int i = 0; i < size; i++) {
      settings.setArrayIndex(i);
      filePaths.push_back(settings.value("path").value<QString>());
    }
    settings.endArray();
    settings.endGroup();
  }
}
void MainWindow::applySettings() {
  for (auto& filePath : filePaths) ui->comboBox_tab1->addItem(filePath);
  if (fileFieldIndex != -1) ui->comboBox_tab1->setCurrentIndex(fileFieldIndex);
  loading_setting_done = true;
}
void MainWindow::choose_file() {
  QString name = qgetenv("USER");
  if (name.isEmpty()) name = qgetenv("USERNAME");
  QString filename = QFileDialog::getOpenFileName(
      this, tr("Open File"), "/Users/" + name, "Objects (*.obj)");
  if (filename.isEmpty()) return;
  filePaths.push_back(filename);
  ui->comboBox_tab1->addItem(filename);
  ui->comboBox_tab1->setCurrentIndex(ui->comboBox_tab1->findText(filename));
}
void clearLayout(QLayout* layout, bool deleteWidgets = true) {
  while (QLayoutItem* item = layout->takeAt(0)) {
    if (deleteWidgets) {
      if (QWidget* widget = item->widget()) widget->close();
    }
    if (QLayout* childLayout = item->layout())
      clearLayout(childLayout, deleteWidgets);
    delete item;
  }
}
void MainWindow::on_pushButton_loadFile_clicked() {
  QString filePath = ui->comboBox_tab1->currentText();
  if (filePath.isEmpty()) return;
  ui->openGLWidget->importObjFile(filePath);
}

void MainWindow::updateInfoLabels(long int vertN, long int edgesN,
                                  QString filename) {
  ui->vetices_l->setText(QString("Total vertices:%1").arg(vertN));
  ui->edges_l->setText(QString("Total edges:%1").arg(edgesN));
  ui->filename_l->setText("File name:" + filename);
}

void MainWindow::on_comboBox_tab1_currentIndexChanged(int index) {
  if (!loading_setting_done) return;
  fileFieldIndex = index;
}

void MainWindow::on_pushButton_saveFile_clicked() {
  QImage screenshot = ui->openGLWidget->grabFramebuffer();
  QString name = qgetenv("USER");
  if (name.isEmpty()) name = qgetenv("USERNAME");
  QString savePath = QFileDialog::getSaveFileName(
      this, tr("Save screenshot"), "/Users/" + name, "Image (*.png)");
  screenshot.save(savePath, "PNG");
}

void MainWindow::saveGif(std::vector<QImage> gifData) {
  QGifImage gif(QSize(640, 480));
  gif.setDefaultDelay(1000 / 10);
  for (auto& frame : gifData) {
    gif.addFrame(frame);
  }
  QString name = qgetenv("USER");
  if (name.isEmpty()) name = qgetenv("USERNAME");
  QString savePath = QFileDialog::getSaveFileName(
      this, tr("Save gif"), "/Users/" + name, "Image (*.gif)");
  gif.save(savePath);
}

void MainWindow::ShowObjectWidget(EObject* object) {
  clearLayout(ui->ObjectWidgetHolder);
  if (object->GetType() != kCamera) ui->openGLWidget->cameraSpacer = nullptr;
  switch (object->GetType()) {
    case kCamera: {
      auto spacer = new CameraSpacer(this, *static_cast<Camera*>(object));
      auto ptr = new CameraConfigView(this, spacer);
      ptr->setAttribute(Qt::WA_DeleteOnClose);
      connect(ptr, SIGNAL(UpdateRequest()), this, SLOT(UpdateGL()));
      ui->ObjectWidgetHolder->addWidget(ptr);
      ui->openGLWidget->cameraSpacer = spacer;
      break;
    }
    case kTransform: {
      auto obj_ptr = static_cast<Transform*>(object);
      auto spacer = new TransformSpacer(*obj_ptr, this);
      auto view = new TransformConfigView(spacer, this);
      view->setAttribute(Qt::WA_DeleteOnClose);
      connect(view, SIGNAL(UpdateRequest()), this, SLOT(UpdateGL()));
      ui->ObjectWidgetHolder->addWidget(view);
    } break;
  }
}
#include "Delegates/CChoicePathDelegate/CChoicePathDelegate.h"
void MainWindow::SetupEObjectTreeView() {
  //  ui->camWid->SetCameraSpacer(ui->openGLWidget->cameraSpacer);
  eObjectModel = &Engine::Instance()->GetEObjectItemModel();

  connect(ui->Tree, &QAbstractItemView::clicked, eObjectModel,
          &EObjectItemModel::FindAndSelectIndex);

  connect(eObjectModel, &EObjectItemModel::ObjectSelected, this,
          &MainWindow::ShowObjectWidget);
  auto delegate = new ComboBoxDelegate (this);
  connect(eObjectModel, &EObjectItemModel::IndexCreated, this, &MainWindow::InsertDeleteBtn);
  ui->Tree->setModel(eObjectModel);
  ui->Tree->setItemDelegate(delegate);
}

void MainWindow::DeleteRow(const QModelIndex &index)
{
  auto ptr = static_cast<EObjectTreeItem*>(index.internalPointer());
  eObjectModel->DeleteItem(ptr);
}

void MainWindow::InsertDeleteBtn(const QModelIndex &index)
{
  bool valid = index.isValid();
  qDebug() << "Inserting delete button";
  auto but = new DeleteButton(index, ui->Tree);
  connect(but, &DeleteButton::ClickedIndex, this, &MainWindow::DeleteRow);
  ui->Tree->setIndexWidget(index, but);
}

void MainWindow::InsertDeleteOnRowsInserted(const QModelIndex &parent, int first, int last)
{
  for (;first <= last; ++first){
    auto child = eObjectModel->index(first, 1, parent);
    auto ptr = static_cast<EObjectTreeItem*>(child.internalPointer());
    InsertDeleteBtn(child);
  }
}

void MainWindow::UpdateGL() { ui->openGLWidget->update(); }
void MainWindow::endCapture() {
  gifBuffer = ui->openGLWidget->stopScreenCapture();
  timerStarted = false;
  saveGif(gifBuffer);
}
void MainWindow::on_pushButton_screencast_clicked() {
  if (timerStarted) return;
  ui->openGLWidget->startScreenCapture(10);
  QTimer::singleShot(5000, this, &MainWindow::endCapture);
  timerStarted = true;
}
void MainWindow::on_pushButton_screencast_auto_clicked() {
  std::vector<QImage> gifData = ui->openGLWidget->getScreencast();
  saveGif(gifData);
}
