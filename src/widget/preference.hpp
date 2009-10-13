/**
 * @file preference.hpp
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
#ifndef KOMIX_WIDGET_PREFERENCE_HPP
#define KOMIX_WIDGET_PREFERENCE_HPP

#include <QDialog>
#include <QSpinBox>
#include <QDialogButtonBox>

namespace KomiX { namespace widget {

/**
 * @brief preference widget
 */
class Preference : public QDialog {
	Q_OBJECT

public:
	Preference( QWidget * parent );


public slots:
	virtual void accept();
	virtual void reject();

private slots:
	void dispatch_( QAbstractButton * );

private:
	void loadSettings_();
	void saveSettings_();

	QSpinBox * step_;
	QSpinBox * interval_;
	QDialogButtonBox * buttons_;
};

} } // end of namespace

#endif
