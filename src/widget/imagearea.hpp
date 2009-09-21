/**
 * @file imagearea.hpp
 * @author Wei-Cheng Pan
 */
#ifndef KOMIX_WIDGET_IMAGEAREA_HPP
#define KOMIX_WIDGET_IMAGEAREA_HPP

#include <QScrollArea>
#include <QPoint>
#include <QLabel>
#include <QSize>
#include <QUrl>

namespace KomiX { namespace widget {

/**
 * @brief The main display area
 * @todo improve the performance of smoothMove()
 *
 * This is the pictures where you see.
 */
class ImageArea : public QScrollArea {
	Q_OBJECT

public:
	/// Viewport position states
	enum ViewPortState {
		/// top-right
		TopRight,
		/// bottom-right
		BottomRight,
		/// top-left
		TopLeft,
		/// bottom-left
		BottomLeft,
	};

	/**
	 * @brief default constructor
	 * @param parent parent widget
	 */
	ImageArea( QWidget * parent = 0 );

public slots:
	/**
	 * @brief set display image
	 * @param image opened image
	 */
	void setImage( const QPixmap & image );

	/**
	 * @brief refresh image size
	 @ @sa scale(int)
	 *
	 * This function will use the setuped status to scale image.\n
	 * The real action function.
	 */
	void scale();

	/**
	 * @brief scale image
	 * @param ratio the scale rate
	 * @sa scale()
	 *
	 * The @p ratio means percents, so 100 actually means 100%.
	 */
	void scale( int ratio );

	/**
	 * @brief move to top by time pass
	 * @sa stepBottom(), stepLeft(), stepRight(), smoothMove()
	 */
	void stepTop();
	/**
	 * @brief move to bottom by time pass
	 * @sa stepTop(), stepLeft(), stepRight(), smoothMove()
	 */
	void stepBottom();
	/**
	 * @brief move to left by time pass
	 * @sa stepTop(), stepBottom(), stepRight(), smoothMove()
	 */
	void stepLeft();
	/**
	 * @brief move to right by time pass
	 * @sa stepTop(), stepBottom(), stepLeft(), smoothMove()
	 */
	void stepRight();
	
	/**
	 * @brief smooth move navigation
	 * @sa stepTop(), stepBottom(), stepLeft(), stepRight()
	 *
	 * The initial state is top-right,
	 * then move to bottom-right,
	 * and then top-left, bottom-left,
	 * open next page finally.
	 */
	void smoothMove();
	void reverseSmoothMove();

	void home();
	void end();

signals:
	void prevPage();
	/// go to next page
	void nextPage();
	/**
	 * @brief whell scroll event
	 * @param distance delta of scrolling
	 */
	void wheelMoved( int distance );
	/**
	 * @brief acceptable file dropped event
	 * @param filePath the droped file path
	 *
	 * Invalid file will not emit this signal.
	 */
	void fileDroped( const QUrl & url );
	/// middle click event
	void middleClicked();

protected:
	/// overrided method
	virtual void mousePressEvent( QMouseEvent * );
	/// overrided method
	virtual void mouseReleaseEvent( QMouseEvent * );
	/// overrided method
	virtual void mouseMoveEvent( QMouseEvent * );
	/// overrided method
	virtual void wheelEvent( QWheelEvent * );
	/// overrided method
	virtual void dragEnterEvent( QDragEnterEvent * );
	/// overrided method
	virtual void dropEvent( QDropEvent * );
	/// overrided method
	virtual void resizeEvent( QResizeEvent * );

private:
	void stopAllStep_();
	void moveScrollBars_( int, int );
	bool canMoveTop_() const;
	bool canMoveBottom_() const;
	bool canMoveLeft_() const;
	bool canMoveRight_() const;

	QLabel * image_;
	QSize imageSize_;
	QTimer * topTimer_;
	QTimer * bottomTimer_;
	QTimer * leftTimer_;
	QTimer * rightTimer_;
	QPoint downPosition_;
	QPoint movePosition_;
	double ratio_;
	int state_;
	int step_;
	int interval_;
};

} } // end namespace

#endif
