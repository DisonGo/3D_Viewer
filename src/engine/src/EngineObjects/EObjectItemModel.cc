#include "E/EObjectItemModel.h"

EObjectItemModel::EObjectItemModel(QObject *parent)
    : QAbstractItemModel{parent} {
  root_item_ = new EObjectTreeItem({tr("Object")}, nullptr);
}

QModelIndex EObjectItemModel::index(int row, int column,
                                    const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent)) return QModelIndex();

  EObjectTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = root_item_;
  else
    parentItem = static_cast<EObjectTreeItem *>(parent.internalPointer());

  EObjectTreeItem *childItem = parentItem->child(row);
  if (childItem) return createIndex(row, column, childItem);
  return QModelIndex();
}

QModelIndex EObjectItemModel::parent(const QModelIndex &index) const {
  if (!index.isValid()) return QModelIndex();

  EObjectTreeItem *childItem =
      static_cast<EObjectTreeItem *>(index.internalPointer());
  EObjectTreeItem *parentItem = childItem->parentItem();

  if (parentItem == root_item_) return QModelIndex();
  if (parentItem == nullptr) return QModelIndex();
  return createIndex(parentItem->row(), 0, parentItem);
}

int EObjectItemModel::rowCount(const QModelIndex &parent) const {
  EObjectTreeItem *parentItem;
  if (parent.column() > 0) return 0;

  if (!parent.isValid())
    parentItem = root_item_;
  else
    parentItem = static_cast<EObjectTreeItem *>(parent.internalPointer());
  return parentItem->childCount();
}

int EObjectItemModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<EObjectTreeItem *>(parent.internalPointer())
        ->columnCount();
  return root_item_->columnCount();
}

Qt::ItemFlags EObjectItemModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags flags = Qt::NoItemFlags;

  if (!index.isValid()) return Qt::NoItemFlags;

  auto *item = static_cast<EObjectTreeItem *>(index.internalPointer());

  auto type = item->GetType();

  if (type == kMesh) return Qt::NoItemFlags;
  flags |= Qt::ItemIsSelectable | Qt::ItemIsEnabled;
  if (type == kObject3D)
    flags &= Qt::ItemIsEnabled;
  if (type == kCamera || type == kObject3D)
    flags |= Qt::ItemIsEditable;

  return flags;
}
QVariant EObjectItemModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return root_item_->data(section);

  return QVariant();
}

QVariant EObjectItemModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();
  EObjectTreeItem *item =
      static_cast<EObjectTreeItem *>(index.internalPointer());

  if (role == Qt::DisplayRole) return item->data(index.column());
  if (role == Qt::EditRole) return item->data(index.column());

  return QVariant();
}

bool EObjectItemModel::setData(const QModelIndex &index, const QVariant &value,
                               int role) {
  if (!index.isValid()) return false;
  if (role != Qt::EditRole) return false;
  EObjectTreeItem *item =
      static_cast<EObjectTreeItem *>(index.internalPointer());
  QVariant val = value;
  QList<QVariant> list;
  list << val;
  item->SetData(list);
  emit dataChanged(index, index);
  return true;
}

std::string EObjectItemModel::GetTitle(EObjectType type) {
  switch (type) {
    case kNone:
      return "";
      break;
    case kCamera:
      return (QString("Camera_") + QString().number(camera_ptrs_.size()))
          .toStdString();
      break;
    case kObject3D:
      return (QString("Object3D_") + QString().number(object3D_ptrs_.size()))
          .toStdString();
      break;
    case kTransform:
//      return (QString("Transform_") + QString().number(transform_ptrs_.size()))
//          .toStdString();
        return "Transform";
      break;
    case kMesh:
//      return (QString("Mesh_") + QString().number(mesh_ptrs_.size()))
//          .toStdString();
        return "Mesh";
      break;
  }
}
void EObjectItemModel::FindAndSelectIndex(const QModelIndex &index) {
  if (!index.isValid()) return;
  EObjectTreeItem *item =
      static_cast<EObjectTreeItem *>(index.internalPointer());
  if (!item) return;
  auto obj_ptr = item->GetObjectPtr();

  if (camera_ptrs_.contains(obj_ptr)) emit ObjectSelected(obj_ptr);
  if (transform_ptrs_.contains(obj_ptr)) emit ObjectSelected(obj_ptr);
}

void EObjectItemModel::PrintIndexObject(const QModelIndex &index) {
  if (!index.isValid()) return;
  EObjectTreeItem *item =
      static_cast<EObjectTreeItem *>(index.internalPointer());
  if (!item) return;
  qDebug() << "Type:" << item->GetType();
  auto obj_ptr = item->GetObjectPtr();
  try {
    if (mesh_ptrs_.contains(obj_ptr))
      qDebug() << " Pointer" << mesh_ptrs_.at(mesh_ptrs_.indexOf(obj_ptr));
    if (camera_ptrs_.contains(obj_ptr))
      qDebug() << " Pointer" << camera_ptrs_.at(camera_ptrs_.indexOf(obj_ptr));
    if (object3D_ptrs_.contains(obj_ptr))
      qDebug() << " Pointer"
               << object3D_ptrs_.at(object3D_ptrs_.indexOf(obj_ptr));
    if (transform_ptrs_.contains(obj_ptr))
      qDebug() << " Pointer"
               << transform_ptrs_.at(transform_ptrs_.indexOf(obj_ptr));
  } catch (...) {
  }
}
void EObjectItemModel::AddItem(EObject *item, EObjectTreeItem *parent) {
  if (!item || item->GetType() == kNone) return;
  if (!parent) parent = root_item_;
  auto type = item->GetType();
  auto row = rowCount();
  string title = GetTitle(type);
  beginInsertRows(QModelIndex(), row + 1, row + 1);
  auto new_item = new EObjectTreeItem({tr(title.c_str())}, item, parent);
  PushObjectInVectors(item);
  endInsertRows();
  if (type == kObject3D) {
    auto object3d = static_cast<Object3D *>(item);
    AddItem(&object3d->GetTrasform(), new_item);
    AddItem(&object3d->GetMesh(), new_item);
  }
}

void EObjectItemModel::PushObjectInVectors(EObject *item) {
  if (!item) return;
  switch (item->GetType()) {
    case kNone:
      return;
      break;
    case kCamera:
      camera_ptrs_ << static_cast<Camera *>(item);
      break;
    case kObject3D:
      object3D_ptrs_ << static_cast<Object3D *>(item);
      break;
    case kTransform:
      transform_ptrs_ << static_cast<Transform *>(item);
      break;
    case kMesh:
      mesh_ptrs_ << static_cast<Mesh *>(item);
      break;
  }
}
