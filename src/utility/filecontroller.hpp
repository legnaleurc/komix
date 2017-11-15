/**
 * @file filecontroller.hpp
 * @author Wei-Cheng Pan
 *
 * KomiX, a comics viewer.
 * Copyright (C) 2008  Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef KOMIX_FILECONTROLLER_HPP
#define KOMIX_FILECONTROLLER_HPP

#include "filemodel.hpp"

#include <memory>


class QModelIndex;


namespace KomiX {

/**
 * @brief The file controller
 *
 * This class is used to provide a generic open image action.
 */
class FileController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief default constructor
     * @param parent parent widget
     */
    explicit FileController(QObject * parent);

    /// check if there has openable files.
    bool isEmpty() const;

    /// get current model
    model::FileModel::SP getModel() const;
    /// get current index
    QModelIndex getCurrentIndex() const;

public slots:
    /// open @p url
    void open(const QUrl & url);
    /// open @p localPath
    void open(const QString & localPath);
    /**
     * @brief open @p index
     *
     * If successful, signal focus(int) is emitted.
     */
    void open(const QModelIndex & index);

signals:
    void modelReady();
    void errorOccured(const QString & message);
    void focus(int id);
    void progressUpdated(int current, int total);

private:
    class Private;
    std::shared_ptr<Private> p_;
};

} // end of KomiX

#endif
