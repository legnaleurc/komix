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
#include <QMessageBox>
#include <QImageReader>
#include <QIcon>
#include <QApplication>
#include <QtDebug>

namespace {

	inline QString formatList() {
		QMultiMap< QString, QString > cat;

		foreach( QString str, KomiX::SupportedFormats() ) {
			if( str == "bmp" ) {
				cat.insert( QObject::tr( "Windows Bitmap" ), str.toLower().prepend( "*." ) );
			} else if( str == "gif" ) {
				cat.insert( QObject::tr( "Graphic Interchange Format" ), str.toLower().prepend( "*." ) );
			} else if( str == "jpg" || str == "jpeg" || str == "jp2" ) {
				cat.insert( QObject::tr( "Joint Photographic Experts Group" ), str.toLower().prepend( "*." ) );
			} else if( str == "mng" || str == "png" ) {
				cat.insert( QObject::tr( "Network Graphics" ), str.toLower().prepend( "*." ) );
			} else if( str == "pbm" || str == "pgm" || str == "ppm" ) {
				cat.insert( QObject::tr( "Portable anymap" ), str.toLower().prepend( "*." ) );
			} else if( str == "tif" || str == "tiff" ) {
				cat.insert( QObject::tr( "Tagged Image File Format" ), str.toLower().prepend( "*." ) );
			} else if( str == "xbm" || str == "xpm" ) {
				cat.insert( QObject::tr( "X11" ), str.toLower().prepend( "*." ) );
			} else if( str == "sgi" ) {
				cat.insert( QObject::tr( "Silicon Graphics Image" ), str.toLower().prepend( "*." ) );
			} else if( str == "tga" ) {
				cat.insert( QObject::tr( "Truevision Advanced Raster Graphics Adapter" ), str.toLower().prepend( "*." ) );
			} else if( str == "eps" || str == "epsf" || str == "epsi" ) {
				cat.insert( QObject::tr( "Encapsulated PostScript" ), str.toLower().prepend( "*." ) );
			} else {
				cat.insert( QObject::tr( "Others" ), str.toLower().prepend( "*." ) );
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

}

namespace KomiX {
	
	const QString MainWindow::fileFilter_ = formatList();
	
	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags f ) :
	QMainWindow( parent, f ),
	imageArea_( new ImageArea( this ) ),
	scaleImage_( new ScaleImage( this ) ),
	preview_( new Preview( this ) ),
	trayIcon_( new QSystemTrayIcon( QIcon( ":/image/logo.svg" ), this ) ),
	dumpState_( Qt::WindowNoState ) {
		initMenuBar_();
		initCentralWidget_();
		initTrayIcon_();
		
		scaleImage_->setWindowTitle( tr( "Scale Image" ) );
		connect( scaleImage_, SIGNAL( scaled( int ) ), imageArea_, SLOT( scale( int ) ) );

		connect( preview_, SIGNAL( open( const QString & ) ), this, SLOT( open( const QString & ) ) );
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
		
		menuBar->addMenu( fileMenu );
		
		QMenu * view = new QMenu( tr( "&View" ), menuBar );
		
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
		connect( jump, SIGNAL( triggered() ), this, SLOT( previewHelper_() ) );

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
		connect( imageArea_, SIGNAL( nextPage() ), &FileController::Instance(), SLOT( next() ) );
		connect( imageArea_, SIGNAL( fileDroped( const QString & ) ), this, SLOT( open( const QString & ) ) );
		connect( imageArea_, SIGNAL( middleClicked() ), this, SLOT( toggleFullScreen() ) );
		connect( &FileController::Instance(), SIGNAL( getImage( const QPixmap & ) ), imageArea_, SLOT( setImage( const QPixmap & ) ) );
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
	
	void MainWindow::systemTrayHelper_( QSystemTrayIcon::ActivationReason reason ) {
		switch( reason ) {
			case QSystemTrayIcon::Trigger:
				toggleSystemTray();
				break;
			default:
				;
		}
	}
	
	void MainWindow::previewHelper_() {
		if( FileController::Instance().isEmpty() ) {
			QMessageBox::information( this, tr( "No file to open" ), tr( "No openable file in this directory." ) );
		} else {
			preview_->listDirectory();
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
		FileController::Instance().open( filePath );
		if( FileController::Instance().isEmpty() ) {
			QMessageBox::information( this, tr( "No file to open" ), tr( "No openable file in this directory." ) );
		}
	}
	
	void MainWindow::openFileDialog() {
		QString filePath = QFileDialog::getOpenFileName( this, tr( "Open image file" ), FileController::Instance().getDirPath(), fileFilter_ );
		if( !filePath.isEmpty() ) {
			open( filePath );
		}
	}
	
	void MainWindow::openDirDialog() {
		QString dirPath = QFileDialog::getExistingDirectory( this, tr( "Open dicrectory" ), FileController::Instance().getDirPath() );
		if( !dirPath.isEmpty() ) {
			open( dirPath );
		}
	}
	
	void MainWindow::toggleFullScreen() {
		menuBar()->setVisible( !menuBar()->isVisible() );
		setWindowState( windowState() ^ Qt::WindowFullScreen );
	}
	
	void MainWindow::toggleSystemTray() {
		if( isVisible() ) {
			dumpState_ = windowState();
			hide();
		} else {
			show();
			setWindowState( dumpState_ );
		}
	}
	
	void MainWindow::about() {
		QMessageBox::about( this, tr( "About KomiX" ), tr( "Hello, world!" ) );
	}

}
