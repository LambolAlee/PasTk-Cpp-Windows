#include "itemdelegate.h"

ItemDelegate::ItemDelegate(UserSizeHintRole role, QObject *parent)
    : QStyledItemDelegate(parent)
    , _role(role)
{

}

QSize ItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (_role) {
    case ItemDelegate::UserSizeHintRole::DetailItemSizeHint :
        return QSize(190, 28);
    case ItemDelegate::UserSizeHintRole::SelectItemSizeHint :
        return QSize(230, 28);
    }
    return QSize();
}
