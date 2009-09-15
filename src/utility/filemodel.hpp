#ifndef KOMIX_FILEMODEL_HPP
#define KOMIX_FILEMODEL_HPP

#include <QFileInfo>
#include <QStringList>
#include <QPixmap>

#include <list>
#include <utility>

namespace KomiX {

	class FileModel {
	public:
		typedef bool ( * KeyFunctor )( const QFileInfo & );
		typedef FileModel * ( * ValueFunctor )( const QFileInfo & );

		static FileModel * createModel( const QFileInfo & path );
		static bool registerModel( const KeyFunctor & key, const ValueFunctor & value );

		bool isEmpty() const;

		QPixmap getImage( int index ) const;
		QPixmap next() const;
		QPixmap prev() const;

	protected:
		void setFileList( const QStringList & files );
		const QStringList & getFileList() const;

	private:
		typedef std::pair< KeyFunctor, ValueFunctor > FunctorPair;
		typedef std::list< FunctorPair > FunctorList;

		class Matcher {
		public:
			Matcher( const QFileInfo & );
			bool operator ()( const FunctorPair & ) const;
		private:
			QFileInfo path_;
		};

		static FunctorList functorList_;

		QStringList files_;
	};

}

#endif // ARCHIVEMODEL_HPP
