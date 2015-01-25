/*
 * (C) Copyright 2014 Alex Spataru
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 */

#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include <QDialog>

namespace Ui
{
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ProgressDialog (QWidget *parent = 0);
        ~ProgressDialog();

    signals:
        void cancelClicked();

    private slots:
        void cancel (void);

    private:
        Ui::ProgressDialog *ui;
};

#endif // PROGRESS_DIALOG_H
