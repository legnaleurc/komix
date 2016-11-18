/**
 * @file asynchronousloader.cpp
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
#include "asynchronousloader.hpp"


namespace KomiX {

class AsynchronousLoader::Private {
public:
    explicit Private(std::shared_ptr<QIODevice> device);

    std::shared_ptr<QIODevice> device;
};
}

using KomiX::AsynchronousLoader;

AsynchronousLoader::Private::Private(std::shared_ptr<QIODevice> device)
    : device(device) {
}

AsynchronousLoader::AsynchronousLoader(std::shared_ptr<QIODevice> device)
    : QObject()
    , QRunnable()
    , p_(new Private(device)) {
}

QIODevice * AsynchronousLoader::getDevice() const {
    return this->p_->device.get();
}
