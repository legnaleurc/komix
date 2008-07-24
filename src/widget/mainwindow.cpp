#include "mainwindow.hpp"

namespace {
	
	QStringList addStar() {
		QStringList temp;
		foreach( QString str, QImageReader::supportedImageFormats() ) {
			temp << str.prepend( "*." );
		}
		return temp;
	}
	
}

namespace KomiX {
	
	const QStringList MainWindow::supportedFormats_ = addStar();
	
	const QString MainWindow::fileFilter_ = MainWindow::supportedFormats_.join( " " );
	
	MainWindow::MainWindow( QWidget * parent, Qt::WindowFlags flags ) : QMainWindow( parent, flags ), imageArea_( new ImageArea( this ) ), scaleImage_( new ScaleImage( this ) ), index_( 0 ), dir_( QDir::home() ), files_() {
		initMenuBar_();
		initCentralWidget_();
		
		connect( scaleImage_, SIGNAL( scaled( int ) ), imageArea_, SLOT( scale( int ) ) );
	}
	
	void MainWindow::initMenuBar_() {
		QPointer< QMenuBar > menuBar = new QMenuBar( this );
		
		QPointer< QMenu > fileMenu = new QMenu( tr( "&File" ), menuBar );
		
		QPointer< QAction > openImage = new QAction( tr( "&Open Image File" ), this );
		openImage->setShortcut( tr( "Ctrl+O" ) );
		connect( openImage, SIGNAL( triggered() ), this, SLOT( openFileDialog() ) );
		
		fileMenu->addAction( openImage );
		addAction( openImage );
		
		QPointer< QAction > openDir = new QAction( tr( "Open &Directory" ), this );
		openDir->setShortcut( tr( "Ctrl+D" ) );
		connect( openDir, SIGNAL( triggered() ), this, SLOT( openDirDialog() ) );
		
		fileMenu->addAction( openDir );
		addAction( openDir );
		
		menuBar->addMenu( fileMenu );
		
		QPointer< QMenu > view = new QMenu( tr( "&View" ), menuBar );
		
		QPointer< QAction > fullScreen = new QAction( tr( "&Full Screen" ), this );
		fullScreen->setShortcut( Qt::Key_F11 );
		connect( fullScreen, SIGNAL( triggered() ), this, SLOT( toggleFullScreen() ) );
		
		view->addAction( fullScreen );
		addAction( fullScreen );
		
		QPointer< QAction > scale = new QAction( tr( "&Scale image" ), this );
		scale->setShortcut( tr( "Ctrl+S" ) );
		connect( scale, SIGNAL( triggered() ), scaleImage_, SLOT( show() ) );
		
		view->addAction( scale );
		addAction( scale );
		
		menuBar->addMenu( view );
		
		QPointer< QMenu > go = new QMenu( tr( "&Go" ), menuBar );
		
		QPointer< QAction > next = new QAction( tr( "&Next image" ), this );
		next->setShortcut( Qt::Key_PageDown );
		connect( next, SIGNAL( triggered() ), this, SLOT( nextFile() ) );
		
		go->addAction( next );
		addAction( next );
		
		QPointer< QAction > prev = new QAction( tr( "&Preverse image" ), this );
		prev->setShortcut( Qt::Key_PageUp );
		connect( prev, SIGNAL( triggered() ), this, SLOT( prevFile() ) );
		
		go->addAction( prev );
		addAction( prev );
		
		menuBar->addMenu( go );
		
		QPointer< QMenu > help = new QMenu( tr( "&Help" ), menuBar );
		
		QPointer< QAction > about__ = new QAction( tr( "&About..." ), this );
		connect( about__, SIGNAL( triggered() ), this, SLOT( about() ) );
		
		help->addAction( about__ );
		addAction( about__ );
		
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
		connect( imageArea_, SIGNAL( nextPage() ), this, SLOT( nextFile() ) );
		connect( imageArea_, SIGNAL( fileDroped( const QString & ) ), this, SLOT( open( const QString & ) ) );
	}
	
	void MainWindow::nextFile() {
		++index_;
		if( index_ >= files_.size() ) {
			index_ = 0;
		}
		imageArea_->openFile( dir_.filePath( files_[index_] ) );
	}
	
	void MainWindow::prevFile() {
		--index_;
		if( index_ < 0 ) {
			index_ = files_.size() - 1;
		}
		imageArea_->openFile( dir_.filePath( files_[index_] ) );
	}
	
	void MainWindow::whellAction( int delta ) {
		if( delta < 0 ) {
			nextFile();
		} else if( delta > 0 ) {
			prevFile();
		}
	}
	
	void MainWindow::updateEnvironment( const QString & name ) {
		QFileInfo temp( name );
		
		if( temp.isDir() ) {
			// Directory mode
			// FIXME: QFileInfo::absoluteDir don't work well if dir path don't have tailing '/'
			dir_ = temp.absoluteDir();
			files_ = dir_.entryList( supportedFormats_, QDir::Files );
			index_ = 0;
		} else {
			// File mode
			dir_ = temp.dir();
			files_ = dir_.entryList( supportedFormats_, QDir::Files );
			index_ = files_.indexOf( temp.fileName() );
		}
	}
	
	void MainWindow::open( const QString & name ) {
		updateEnvironment( name );
		imageArea_->openFile( dir_.filePath( files_[index_] ) );
	}
	
	void MainWindow::openFileDialog() {
		QString fileName = QFileDialog::getOpenFileName( this, tr( "Open image file" ), dir_.absolutePath(), fileFilter_ );
		if( !fileName.isEmpty() ) {
			open( fileName );
		}
	}
	
	void MainWindow::openDirDialog() {
		QString dirName = QFileDialog::getExistingDirectory( this, tr( "Open dicrectory" ), dir_.absolutePath() );
		if( !dirName.isEmpty() ) {
			open( dirName );
		}
	}
	
	void MainWindow::toggleFullScreen() {
		if( isFullScreen() ) {
			menuBar()->show();
			showNormal();
		} else {
			menuBar()->hide();
			showFullScreen();
		}
	}
	
	void MainWindow::about() {
		QMessageBox::about( this, tr( "About KomiX" ), tr( "Hello, world!" ) );
	}

}
