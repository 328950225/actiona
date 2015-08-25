/*
    Actiona
    Copyright (C) 2008-2015 Jonathan Mercier-Ganady

    Actiona is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Actiona is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

    Contact : jmgr@jmgr.info
*/

#ifndef NEWACTIONPROXYMODEL_HPP
#define NEWACTIONPROXYMODEL_HPP

#include <QSortFilterProxyModel>

class NewActionProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    NewActionProxyModel(QObject *parent = 0);

    void setFilterString(const QString &filterString);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QString mFilterString;
};

#endif // NEWACTIONPROXYMODEL_HPP
