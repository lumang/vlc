/*****************************************************************************
 * GotoTime.cpp : GotoTime and About dialogs
 ****************************************************************************
 * Copyright (C) 2007 the VideoLAN team
 * $Id: Messages.cpp 16024 2006-07-13 13:51:05Z xtophe $
 *
 * Authors: Jean-Baptiste Kempf <jb (at) videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#include "dialogs/gototime.hpp"

#include "dialogs_provider.hpp"
#include "main_interface.hpp"
#include "util/qvlcframe.hpp"
#include "qt4.hpp"
#include "input_manager.hpp"

#include <QTabWidget>
#include <QFile>
#include <QLabel>
#include <QTimeEdit>
#include <QGroupBox>
#include <QDialogButtonBox>

GotoTimeDialog *GotoTimeDialog::instance = NULL;

GotoTimeDialog::GotoTimeDialog( intf_thread_t *_p_intf) :  QVLCFrame( _p_intf )
{
    setWindowFlags( Qt::Tool );
    setWindowTitle( qtr( "Go to time" ) );
    resize( 260, 160 );

    QGridLayout *mainLayout = new QGridLayout( this );

    QPushButton *gotoButton = new QPushButton( qtr( "&Go" ) );
    QPushButton *cancelButton = new QPushButton( qtr( "&Cancel" ) );
    QDialogButtonBox *buttonBox = new QDialogButtonBox;

    gotoButton->setDefault( true );
    buttonBox->addButton( gotoButton, QDialogButtonBox::AcceptRole );
    buttonBox->addButton( cancelButton, QDialogButtonBox::RejectRole );

    QGroupBox *timeGroupBox = new QGroupBox;
    QGridLayout *boxLayout = new QGridLayout( timeGroupBox );

    QLabel *timeIntro = new
        QLabel( "Go to time:" );
    timeIntro->setWordWrap( true );
    timeIntro->setAlignment( Qt::AlignCenter );

    timeEdit = new QTimeEdit();
    timeEdit->setDisplayFormat( "hh : mm : ss" );
    timeEdit->setAlignment( Qt::AlignRight );
    timeEdit->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Minimum );

    QLabel *helpFormat = new QLabel( timeEdit->displayFormat() );
    helpFormat->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Preferred );

    QSpacerItem *spacerBox = new QSpacerItem(20, 10, QSizePolicy::Minimum,
                                        QSizePolicy::Fixed);

    QSpacerItem *spacerItem = new QSpacerItem(20, 3, QSizePolicy::Minimum,
                                        QSizePolicy::Expanding);

    boxLayout->addWidget( timeIntro, 0, 0, 1, 2 );
    boxLayout->addItem( spacerBox, 1, 0, 1, 2 );
    boxLayout->addWidget( timeEdit, 2, 0, 1, 1 );
    boxLayout->addWidget( helpFormat, 2, 1, 1, 1 );

    mainLayout->addWidget( timeGroupBox, 0, 0, 1, 4 );
    mainLayout->addItem( spacerItem, 1, 0 );
    mainLayout->addWidget( buttonBox, 2, 3 );

    BUTTONACT( gotoButton, close() );
    BUTTONACT( cancelButton, cancel() );
}

GotoTimeDialog::~GotoTimeDialog()
{
}

void GotoTimeDialog::cancel()
{
    timeEdit->setTime( QTime( 0, 0, 0) );
    this->toggleVisible();
}

void GotoTimeDialog::close()
{
    if ( THEMIM->getIM()->hasInput() )
    {
        int64_t i_time = (int64_t)
            ( QTime( 0, 0, 0 ).msecsTo( timeEdit->time() ) ) * 1000;
        var_SetTime( THEMIM->getInput(), "time", i_time );
    }
    this->toggleVisible();
}
