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
    auto & global = KomiX::Global::instance();
    const auto & supported = global.getSupportedFormats();
    if (!supported.contains(suffix)) {
        return nullptr;
    }
    return std::make_shared<SingleModel>(fi);
}


void SingleModel::registerDialogFilter() {
    auto & global = KomiX::Global::instance();
    auto builder = global.createDialogFilterBuilder();

    builder.setSummary(QObject::tr("All Supported Image File"));

    for (auto str : global.getSupportedFormats()) {
        if (str == "bmp") {
            builder.addFilter(QObject::tr("Windows Bitmap"), str);
        } else if (str == "gif") {
            builder.addFilter(QObject::tr("Graphic Interchange Format"), str);
        } else if (str == "jpg" || str == "jpeg" || str == "jp2") {
            builder.addFilter(QObject::tr("Joint Photographic Experts Group"), str);
        } else if (str == "mng" || str == "png") {
            builder.addFilter(QObject::tr("Network Graphics"), str);
        } else if (str == "pbm" || str == "pgm" || str == "ppm") {
            builder.addFilter(QObject::tr("Portable anymap"), str);
        } else if (str == "tif" || str == "tiff") {
            builder.addFilter(QObject::tr("Tagged Image File Format"), str);
        } else if (str == "xbm" || str == "xpm") {
            builder.addFilter(QObject::tr("X11"), str);
        } else if (str == "sgi" || str == "rgba" || str == "rgb" || str == "bw") {
            builder.addFilter(QObject::tr("Silicon Graphics Image"), str);
        } else if (str == "tga") {
            builder.addFilter(QObject::tr("Truevision Advanced Raster Graphics Adapter"), str);
        } else if (str == "eps" || str == "epsf" || str == "epsi") {
            builder.addFilter(QObject::tr("Encapsulated PostScript"), str);
        } else if (str == "svg") {
            builder.addFilter(QObject::tr("Scalable Vector Graphics"), str);
        } else if (str == "xcf") {
            builder.addFilter(QObject::tr("eXperimental Computing Facility"), str);
        } else if (str == "psd") {
            builder.addFilter(QObject::tr("Photoshop Document"), str);
        } else if (str == "pcx") {
            builder.addFilter(QObject::tr("PC Paintbrush Exchange"), str);
        } else if (str == "ico") {
            builder.addFilter(QObject::tr("Windows icon"), str);
        } else if (str == "dds") {
            builder.addFilter(QObject::tr("DirectDraw Surface"), str);
        } else if (str == "exr") {
            builder.addFilter(QObject::tr("OpenEXR"), str);
        } else {
            builder.addFilter(QObject::tr("Others"), str);
        }
    }
}


SingleModel::SingleModel(const QFileInfo & root)
    : LocalFileModel()
{
    this->setRoot(root.dir());
}
