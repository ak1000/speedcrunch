// This file is part of the SpeedCrunch project
// Copyright (C) 2007 Petri Damstén <damu@iki.fi>
// Copyright (C) 2008 Helder Correia <helder.pereira.correia@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.


#include <gui/bookdock.hxx>

#include <QComboBox>
#include <QFile>
#include <QLibraryInfo>
#include <QLocale>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>


struct BookDock::Private
{
  QComboBox *    sheets;
  QTextBrowser * sheet;
  QString        path;
  QString        lang;
};


BookDock::BookDock( QWidget * parent )
  : QDockWidget( tr("Book"), parent ),
    d( new BookDock::Private )
{
  d->path = BOOKSDIR;
  QString locale = QLocale().name();
  QString localeShort = locale.left( 2 ).toLower();
  d->lang = localeShort;

  QWidget* widget = new QWidget( this );
  QVBoxLayout* bookLayout = new QVBoxLayout;

  d->sheet = new QTextBrowser( this );
  //d->sheet->setOpenLinks(false); // remove to stick with Qt 4.2
  d->sheet->setSearchPaths( QStringList() << d->path );
  connect( d->sheet, SIGNAL( anchorClicked ( const QUrl & ) ),
           this, SLOT( anchorClicked ( const QUrl & ) ) );

  QHBoxLayout * buttonLayout = new QHBoxLayout;
  buttonLayout->setSpacing( 0 );
  buttonLayout->setMargin( 0 );
  QPushButton * button = new QPushButton( this );
  button->setIcon( QPixmap( ":/book_back.png" ) );
  button->setFlat( true );
  button->setToolTip( tr("Back") );
  connect( button, SIGNAL( clicked() ), d->sheet, SLOT( backward() ) );
  buttonLayout->addWidget( button );

  button = new QPushButton( this );
  button->setIcon( QPixmap( ":/book_forward.png" ) );
  button->setFlat( true );
  button->setToolTip( tr("Forward") );
  connect( button, SIGNAL( clicked() ), d->sheet, SLOT( forward() ) );
  buttonLayout->addWidget( button );

  button = new QPushButton( this );
  button->setIcon( QPixmap( ":/book_home.png" ) );
  button->setFlat( true );
  button->setToolTip( tr("Home") );
  connect( button, SIGNAL( clicked() ), d->sheet, SLOT( home() ) );
  buttonLayout->addWidget( button );

  buttonLayout->addStretch();
  bookLayout->addLayout( buttonLayout );
  bookLayout->addWidget( d->sheet );

  widget->setLayout( bookLayout );
  setWidget( widget );

  setMinimumWidth( 150 );
  setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
  setWindowIcon( QIcon() ); // no icon

  d->sheet->setSource( d->path + d->lang + "/index.html" );
}


BookDock::~BookDock()
{
  delete d;
}


void BookDock::anchorClicked ( const QUrl & link )
{
  if ( link.toString().startsWith( "file:#" ) )
  {
    emit expressionSelected( link.toString().mid( 6 ) );
  }
  else
  {
    d->sheet->setSource( link );
  }
}
