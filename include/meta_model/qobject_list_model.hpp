#pragma once

#include <type_traits>

#include "meta_model/qgadget_helper.hpp"
#include "meta_model/qmeta_list_model.hpp"

namespace meta_model
{

namespace detail
{
template<typename T>
    requires std::is_base_of_v<QObject, T>
class QObjectListModel : public QMetaListModel<T*, QObjectListModel<T>, QMetaListStore::Vector> {
    template<typename, QMetaListStore, typename, typename>
    friend class detail::QMetaListModelPre;

public:
    using base_type    = QMetaListModel<T*, QObjectListModel<T>, QMetaListStore::Vector>;
    QObjectListModel() = delete;

    template<typename U = T>
        requires std::same_as<U, QObject>
    QObjectListModel(const QMetaObject& meta, bool is_owner, QObject* parent = nullptr)
        : base_type(parent), m_is_owner(is_owner) {
        this->updateRoleNames(meta);
    }
    template<typename U = T>
        requires(! std::same_as<U, QObject>)
    QObjectListModel(bool is_owner, QObject* parent = nullptr)
        : base_type(parent), m_is_owner(is_owner) {
        this->updateRoleNames(T::staticMetaObject);
    }
    virtual ~QObjectListModel() {}

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
        if (auto prop = this->propertyOfRole(role); prop) {
            return prop.value().read(this->at(index.row()));
        }
        return {};
    };

private:
    template<typename Tin>
        requires std::convertible_to<typename std::iterator_traits<Tin>::value_type, T*>
    void _insert_impl(std::size_t pos, Tin beg, Tin end) {
        if (m_is_owner) {
            auto it = beg;
            for (; it < end; it++) {
                (*it)->setParent(this);
            }
        }
        base_type::_insert_impl(pos, beg, end);
    }

    bool m_is_owner;
};
} // namespace detail

using QObjectListModel = detail::QObjectListModel<QObject>;
} // namespace meta_model