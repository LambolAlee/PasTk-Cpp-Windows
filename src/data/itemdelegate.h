#ifndef ITEMDELEGATE_H
#define ITEMDELEGATE_H

#include <QStyledItemDelegate>

class ItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum UserSizeHintRole {
        DetailItemSizeHint = Qt::UserRole,
        SelectItemSizeHint,
    };

    ItemDelegate(UserSizeHintRole role, QObject *parent = nullptr);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    UserSizeHintRole _role;
};

#endif // ITEMDELEGATE_H
