#include "mainwindow.hpp"
#include "scaleimage.hpp"
#include "imagearea.hpp"
#include "preview.hpp"
#include "global.hpp"
#include "filecontroller.hpp"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QTabWidget>
#include <QMessageBox>
#include <QApplication>
#include <QtDebug>
#include <QVBoxLayout>

namespace {

	inline QString formatList() {
		QMultiMap< QString, QString > cat;

		foreach( QString str, KomiX::SupportedFormats() ) {
			if( str == "bmp" ) {
				cat.insert( QObject::tr( "Windows Bitmap" ), str.prepend( "*." ) );
			} else if( str == "gif" ) {
				cat.insert( QObject::tr( "Graphic Interchange Format" ), str.prepend( "*." ) );
			} else if( str == "jpg" || str == "jpeg" || str == "jp2" ) {
				cat.insert( QObject::tr( "Joint Photographic Experts Group" ), str.prepend( "*." ) );
			} else if( str == "mng" || str == "png" ) {
				cat.insert( QObject::tr( "Network Graphics" ), str.prepend( "*." ) );
			} else if( str == "pbm" || str == "pgm" || str == "ppm" ) {
				cat.insert( QObject::tr( "Portable anymap" ), str.prepend( "*." ) );
			} else if( str == "tif" || str == "tiff" ) {
				cat.insert( QObject::tr( "Tagged Image File Format" ), str.prepend( "*." ) );
			} else if( str == "xbm" || str == "xpm" ) {
				cat.insert( QObject::tr( "X11" ), str.prepend( "*." ) );
			} else if( str == "sgi" || str == "rgba" || str == "rgb" || str == "bw" ) {
				cat.insert( QObject::tr( "Silicon Graphics Image" ), str.prepend( "*." ) );
			} else if( str == "tga" ) {
				cat.insert( QObject::tr( "Truevision Advanced Raster Graphics Adapter" ), str.prepend( "*." ) );
			} else if( str == "eps" || str == "epsf" || str == "epsi" ) {
				cat.insert( QObject::tr( "Encapsulated PostScript" ), str.prepend( "*." ) );
			} else if( str == "svg" ) {
				cat.insert( QObject::tr( "Scalable Vector Graphics" ), str.prepend( "*." ) );
			} else if( str == "xcf" ) {
				cat.insert( QObject::tr( "eXperimental Computing Facility" ), str.prepend( "*." ) );
			} else if( str == "psd" ) {
				cat.insert( QObject::tr( "Photoshop Document" ), str.prepend( "*." ) );
			} else if( str == "pcx" ) {
				cat.insert( QObject::tr( "PC Paintbrush Exchange" ), str.prepend( "*." ) );
			} else if( str == "ico" ) {
				cat.insert( QObject::tr( "Windows icon" ), str.prepend( "*." ) );
			} else if( str == "dds" ) {
				cat.insert( QObject::tr( "DirectDraw Surface" ), str.prepend( "*." ) );
			} else if( str == "exr" ) {
				cat.insert( QObject::tr( "OpenEXR" ), str.prepend( "*." ) );
			} else {
				cat.insert( QObject::tr( "Others" ), str.prepend( "*." ) );
			}
		}

		QStringList tmp;
		foreach( QString key, cat.uniqueKeys() ) {
			tmp.push_back( key + " ( " + QStringList( cat.values( key ) ).join( " " ) + " )" );
		}
		tmp.push_front( QObject::tr( "All Supported File" ) + " ( " + KomiX::SupportedFormatsFilter().join( " " ) + " )" );

		qDebug() << tmp;
		return tmp.join( ";;" );
	}

	inline QString archiveList() {
		QStringList tmp( QObject::tr( "All Supported Archives ( %1 )" ).arg( KomiX::ArchiveFormatsFilter().join( " " ) ) );
		tmp << KomiX::ArchiveFormatsFilter();
		qDebug() << tmp;
		return tmp.join( ";;" );
	}

}

namespace KomiX {

	inline const QString & MainWindow::fileFilter_() {
		static QString ff = formatList();
		return ff;
	}

	inline const QString & MainWindow::archiveFilter_() {
		static QString af = archiveList();
		return af;
	}

	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags f ) :
	QMainWindow( parent, f ),
	imageArea_( new ImageArea( this ) ),
	scaleImage_( new ScaleImage( this ) ),
	preview_( new Preview( this ) ),
	trayIcon_( new QSystemTrayIcon( QIcon( ":/image/logo.svg" ), this ) ),
	about_( new QWidget( this, Qt::Dialog ) ),
	dumpState_( Qt::WindowNoState ) {
		initMenuBar_();
		initCentralWidget_();
		initTrayIcon_();
		initAbout_();

		scaleImage_->setWindowTitle( tr( "Scale Image" ) );
		connect( scaleImage_, SIGNAL( scaled( int ) ), imageArea_, SLOT( scale( int ) ) );

		connect( preview_, SIGNAL( required( const QModelIndex & ) ), this, SLOT( open( const QModelIndex & ) ) );
	}

	void MainWindow::initMenuBar_() {
		QMenuBar * menuBar = new QMenuBar( this );

		QMenu * fileMenu = new QMenu( tr( "&File" ), menuBar );

		QAction * openImage = new QAction( tr( "&Open Image File" ), this );
		openImage->setShortcut( tr( "Ctrl+O" ) );
		connect( openImage, SIGNAL( triggered() ), this, SLOT( openFileDialog() ) );

		fileMenu->addAction( openImage );
		addAction( openImage );

		QAction * openDir = new QAction( tr( "Open &Directory" ), this );
		openDir->setShortcut( tr( "Ctrl+D" ) );
		connect( openDir, SIGNAL( triggered() ), this, SLOT( openDirDialog() ) );

		fileMenu->addAction( openDir );
		addAction( openDir );

		QAction * openArchive = new QAction( tr( "Open &Archive" ), this );
		openArchive->setShortcut( tr( "Ctrl+A" ) );
		connect( openArchive, SIGNAL( triggered() ), this, SLOT( openArchiveDialog() ) );

		fileMenu->addAction( openArchive );
		addAction( openArchive );

		menuBar->addMenu( fileMenu );

		QMenu * view = new QMenu( tr( "&View" ), menuBar );

		QAction * smoothNext = new QAction( tr( "Smooth &Next" ), this );
		smoothNext->setShortcut( Qt::Key_Space );
		connect( smoothNext, SIGNAL( triggered() ), this->imageArea_, SLOT( smoothMove() ) );
		view->addAction( smoothNext );
		addAction( smoothNext );

		QAction * smoothPrev = new QAction( tr( "Smooth &Prev" ), this );
		smoothPrev->setShortcut( Qt::Key_Return );
		connect( smoothPrev, SIGNAL( triggered() ), this->imageArea_, SLOT( reverseSmoothMove() ) );
		view->addAction( smoothPrev );
		addAction( smoothPrev );

		QAction * home = new QAction( tr( "Page &Head" ), this );
		home->setShortcut( Qt::Key_Home );
		connect( home, SIGNAL( triggered() ), this->imageArea_, SLOT( home() ) );
		view->addAction( home );
		addAction( home );

		QAction * end = new QAction( tr( "Page &Tail" ), this );
		end->setShortcut( Qt::Key_End );
		connect( end, SIGNAL( triggered() ), this->imageArea_, SLOT( end() ) );
		view->addAction( end );
		addAction( end );

		view->addSeparator();

		QAction * fullScreen = new QAction( tr( "&Full Screen" ), this );
		fullScreen->setShortcut( Qt::Key_F11 );
		connect( fullScreen, SIGNAL( triggered() ), this, SLOT( toggleFullScreen() ) );
		view->addAction( fullScreen );
		addAction( fullScreen );

		QAction * hide = new QAction( tr( "&Hide window" ), this );
		hide->setShortcut( tr( "Esc" ) );
		connect( hide, SIGNAL( triggered() ), this, SLOT( toggleSystemTray() ) );
		view->addAction( hide );
		addAction( hide );

		view->addSeparator();

		QAction * scale = new QAction( tr( "&Scale image" ), this );
		scale->setShortcut( tr( "Ctrl+S" ) );
		connect( scale, SIGNAL( triggered() ), scaleImage_, SLOT( show() ) );
		view->addAction( scale );
		addAction( scale );

		menuBar->addMenu( view );

		QMenu * go = new QMenu( tr( "&Go" ), menuBar );

		QAction * jump = new QAction( tr( "&Jump to image" ), this );
		jump->setShortcut( tr( "Ctrl+J" ) );
		connect( jump, SIGNAL( triggered() ), preview_, SLOT( popup() ) );
		go->addAction( jump );
		addAction( jump );

		go->addSeparator();

		QAction * prev = new QAction( tr( "&Preverse image" ), this );
		prev->setShortcut( Qt::Key_PageUp );
		connect( prev, SIGNAL( triggered() ), &FileController::Instance(), SLOT( prev() ) );
		go->addAction( prev );
		addAction( prev );

		QAction * next = new QAction( tr( "&Next image" ), this );
		next->setShortcut( Qt::Key_PageDown );
		connect( next, SIGNAL( triggered() ), &FileController::Instance(), SLOT( next() ) );
		go->addAction( next );
		addAction( next );

		menuBar->addMenu( go );

		QMenu * help = new QMenu( tr( "&Help" ), menuBar );

		QAction * about__ = new QAction( tr( "&About" ), this );
		connect( about__, SIGNAL( triggered() ), this, SLOT( about() ) );

		help->addAction( about__ );

		QAction * aboutQt_ = new QAction( tr( "About &Qt" ), this );
		connect( aboutQt_, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

		help->addAction( aboutQt_ );

		menuBar->addMenu( help );

		setMenuBar( menuBar );
	}

	void MainWindow::initCentralWidget_() {
		setCentralWidget( imageArea_ );

		imageArea_->setBackgroundRole( QPalette::Dark );
		imageArea_->setAlignment( Qt::AlignCenter );
		imageArea_->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
		imageArea_->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
		imageArea_->setAcceptDrops( true );

		connect( imageArea_, SIGNAL( wheelMoved( int ) ), this, SLOT( whellAction( int ) ) );
		connect( imageArea_, SIGNAL( prevPage() ), &FileController::Instance(), SLOT( prev() ) );
		connect( imageArea_, SIGNAL( nextPage() ), &FileController::Instance(), SLOT( next() ) );
		connect( imageArea_, SIGNAL( fileDroped( const QString & ) ), this, SLOT( open( const QString & ) ) );
		connect( imageArea_, SIGNAL( middleClicked() ), this, SLOT( toggleFullScreen() ) );
		connect( &FileController::Instance(), SIGNAL( imageLoaded( const QPixmap & ) ), imageArea_, SLOT( setImage( const QPixmap & ) ) );
	}

	void MainWindow::initTrayIcon_() {
		QMenu * menu = new QMenu( this );
		QAction * quit = menu->addAction( tr( "&Quit" ) );
		trayIcon_->setContextMenu( menu );

		trayIcon_->setToolTip( tr( "KomiX" ) );

		connect( quit, SIGNAL( triggered() ), qApp, SLOT( quit() ) );
		connect( trayIcon_, SIGNAL( activated( QSystemTrayIcon::ActivationReason ) ), this, SLOT( systemTrayHelper_( QSystemTrayIcon::ActivationReason ) ) );

		trayIcon_->show();
	}

	void MainWindow::initAbout_() {
		about_->setWindowTitle( tr( "About KomiX" ) );

		QVBoxLayout * outer = new QVBoxLayout( about_ );
		about_->setLayout( outer );

		QHBoxLayout * head = new QHBoxLayout();
		outer->addLayout( head );

		QLabel * logo = new QLabel( about_ );
		logo->setPixmap( QPixmap( ":/image/logo.svg" ).scaled( 60, 60 ) );
		head->addWidget( logo );

		QLabel * version = new QLabel( about_ );
		version->setText( tr(
			"<h1>KomiX</h1>"
			"Version: 0.0.70<br/>"
			"<a href=\"http://legnaleurc.blogspot.com/search/label/KomiX/\">More information</a>"
		) );
		version->setTextFormat( Qt::RichText );
		head->addWidget( version );

		QTabWidget * tabPages = new QTabWidget( about_ );
		outer->addWidget( tabPages );

		QLabel * about__ = new QLabel( about_ );
		about__->setText( tr(
			"KomiX - A comics viewer\n"
			"\n"
			"(c) 2008-2009 FoolproofProject\n"
			"License: GPLv3\n"
		) );
		tabPages->addTab( about__, tr( "&About" ) );

		QLabel * authors = new QLabel( about_ );
		authors->setText( tr(
			"<h6>Wei-Cheng Pan (FoolproofProject)</h6><br/>"
			"Site: <a href=\"http://legnaleurc.blogspot.com/\">legnaleurc.blogspot.com</a><br/>"
			"E-Mail: <a href=\"mailto:legnaleurc@gmail.com\">legnaleurc@gmail.com</a><br/>"
		) );
		authors->setTextFormat( Qt::RichText );
		tabPages->addTab( authors, tr( "A&uthors" ) );

		QLabel * aboutContent = new QLabel( about_ );
		aboutContent->setPixmap( QPixmap( ":/image/womm.png" ) );
		tabPages->addTab( aboutContent, tr( "&Certification" ) );
	}

	void MainWindow::systemTrayHelper_( QSystemTrayIcon::ActivationReason reason ) {
		switch( reason ) {
			case QSystemTrayIcon::Trigger:
				toggleSystemTray();
				break;
			default:
				;
		}
	}

	void MainWindow::whellAction( int delta ) {
		if( delta < 0 ) {
			FileController::Instance().next();
		} else if( delta > 0 ) {
			FileController::Instance().prev();
		}
	}

	void MainWindow::open( const QString & filePath ) {
		if( !FileController::Instance().open( filePath ) ) {
			QMessageBox::information( this, tr( "No file to open" ), tr( "No openable file in this directory." ) );
		}
	}

	void MainWindow::open( const QModelIndex & item ) {
		imageArea_->setImage( item.data( Qt::UserRole ).value< QPixmap >() );
	}

	void MainWindow::openFileDialog() {
// 		qDebug( "<MainWindow::openFileDialog()>" );
// 		qDebug() << fileFilter_();
// 		qDebug( "</MainWindow::openFileDialog()>" );
		// FIXME
		QString filePath = QFileDialog::getOpenFileName( this, tr( "Open image file" ), QDir::homePath(), fileFilter_() );
		if( !filePath.isEmpty() ) {
			open( filePath );
		}
	}

	void MainWindow::openDirDialog() {
		// FIXME
		QString dirPath = QFileDialog::getExistingDirectory( this, tr( "Open dicrectory" ), QDir::homePath() );
		if( !dirPath.isEmpty() ) {
			open( dirPath );
		}
	}

	void MainWindow::openArchiveDialog() {
		// FIXME
		QString archivePath = QFileDialog::getOpenFileName( this, tr( "Open archive" ), QDir::homePath(), archiveFilter_() );
		if( !archivePath.isEmpty() ) {
			open( archivePath );
		}
	}

	void MainWindow::toggleFullScreen() {
// 		qDebug( "<MainWindow::toggleFullScreen()>" );
// 		qDebug() << "before: " << windowState();
		menuBar()->setVisible( !menuBar()->isVisible() );
		setWindowState( windowState() ^ Qt::WindowFullScreen );
// 		qDebug() << "after: " << windowState();
// 		qDebug( "</MainWindow::toggleFullScreen()>" );
	}

	void MainWindow::toggleSystemTray() {
// 		qDebug( "<MainWindow::toggleSystemTray()>" );
// 		qDebug() << "before: " << windowState();
		if( isVisible() ) {
			dumpState_ = windowState();
			hide();
		} else {
			show();
			setWindowState( dumpState_ );
		}
// 		qDebug() << "after: " << windowState();
// 		qDebug( "</MainWindow::toggleSystemTray()>" );
	}

	void MainWindow::about() {
		about_->show();
	}

}
