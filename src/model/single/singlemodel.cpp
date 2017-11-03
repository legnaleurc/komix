/**
 * @file singlemodel.cpp
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
#include "singlemodel.hpp"
#include "global.hpp"


using namespace KomiX::model;


FileModel::SP SingleModel::create(const QUrl & url) {
    if (url.scheme() != "file") {
        return nullptr;
    }
    QFileInfo fi(url.toLocalFile());
    if (fi.isDir()) {
        return nullptr;
    }
    QString suffix = fi.suffix().toLower();
    const auto & supported = KomiX::SupportedFormats();
    if (!supported.contains(suffix)) {
        return nullptr;
    }
    return std::make_shared<SingleModel>(fi);
}


QString SingleModel::createDialogFilter() {
    QMultiMap<QString, QString> cat;

    for (auto str : KomiX::SupportedFormats()) {
        if (str == "bmp") {
            cat.insert(QObject::tr("Windows Bitmap"), str.prepend("*."));
        } else if (str == "gif") {
            cat.insert(QObject::tr("Graphic Interchange Format"), str.prepend("*."));
        } else if (str == "jpg" || str == "jpeg" || str == "jp2") {
            cat.insert(QObject::tr("Joint Photographic Experts Group"), str.prepend("*."));
        } else if (str == "mng" || str == "png") {
            cat.insert(QObject::tr("Network Graphics"), str.prepend("*."));
        } else if (str == "pbm" || str == "pgm" || str == "ppm") {
            cat.insert(QObject::tr("Portable anymap"), str.prepend("*."));
        } else if (str == "tif" || str == "tiff") {
            cat.insert(QObject::tr("Tagged Image File Format"), str.prepend("*."));
        } else if (str == "xbm" || str == "xpm") {
            cat.insert(QObject::tr("X11"), str.prepend("*."));
        } else if (str == "sgi" || str == "rgba" || str == "rgb" || str == "bw") {
            cat.insert(QObject::tr("Silicon Graphics Image"), str.prepend("*."));
        } else if (str == "tga") {
            cat.insert(QObject::tr("Truevision Advanced Raster Graphics Adapter"), str.prepend("*."));
        } else if (str == "eps" || str == "epsf" || str == "epsi") {
            cat.insert(QObject::tr("Encapsulated PostScript"), str.prepend("*."));
        } else if (str == "svg") {
            cat.insert(QObject::tr("Scalable Vector Graphics"), str.prepend("*."));
        } else if (str == "xcf") {
            cat.insert(QObject::tr("eXperimental Computing Facility"), str.prepend("*."));
        } else if (str == "psd") {
            cat.insert(QObject::tr("Photoshop Document"), str.prepend("*."));
        } else if (str == "pcx") {
            cat.insert(QObject::tr("PC Paintbrush Exchange"), str.prepend("*."));
        } else if (str == "ico") {
            cat.insert(QObject::tr("Windows icon"), str.prepend("*."));
        } else if (str == "dds") {
            cat.insert(QObject::tr("DirectDraw Surface"), str.prepend("*."));
        } else if (str == "exr") {
            cat.insert(QObject::tr("OpenEXR"), str.prepend("*."));
        } else {
            cat.insert(QObject::tr("Others"), str.prepend("*."));
        }
    }

    QStringList tmp;
    for (const auto & key : cat.uniqueKeys()) {
        QStringList values = cat.values(key);
        QString filter = "%1 ( %2 )";
        tmp.push_back(filter.arg(key).arg(values.join(" ")));
    }
    const auto & all = KomiX::SupportedFormatsFilter();
    QString filter = QObject::tr("All Supported Image File ( %1 )");
    tmp.push_front(filter.arg(all.join(" ")));

    return tmp.join(";;");
}


SingleModel::SingleModel(const QFileInfo & root)
    : LocalFileModel()
{
    this->setRoot(root.dir());
}
