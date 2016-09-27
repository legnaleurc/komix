/**
 * @file blockdeviceloader.cpp
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
#include "blockdeviceloader.hpp"

#include <cassert>


using KomiX::BlockDeviceLoader;


BlockDeviceLoader::BlockDeviceLoader(std::shared_ptr<QIODevice> device)
    : AsynchronousLoader(device) {
}

void BlockDeviceLoader::run() {
    auto device = this->getDevice();
    if (!device->open(QIODevice::ReadOnly)) {
        // TODO emit error signal
        assert(!"can not open file");
        return;
    }
    auto bytes = device->readAll();
    device->close();
    emit this->finished(bytes);
}
