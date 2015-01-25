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

#include "progress_dialog.h"
#include "ui_progress_dialog.h"

ProgressDialog::ProgressDialog (QWidget *parent) : QDialog (parent), ui (new Ui::ProgressDialog)
{
    // Create and configure UI
    ui->setupUi (this);

    // Make the window look like a dialog
    QIcon _blank;
    setWindowIcon (_blank);
    setWindowModality (Qt::WindowModal);
    setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Close dialog when cancel button is clicked
    connect (ui->ui_cancel_button, SIGNAL (clicked()), this, SLOT (cancel()));
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::cancel (void)
{
    hide();
    emit cancelClicked();
}
