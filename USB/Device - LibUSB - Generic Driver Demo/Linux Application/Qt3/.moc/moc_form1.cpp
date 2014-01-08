/****************************************************************************
** Form1 meta object code from reading C++ file 'form1.h'
**
** Created: Thu Jun 2 20:18:45 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.8   edited Feb 2 14:59 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../.ui/form1.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.8b. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Form1::className() const
{
    return "Form1";
}

QMetaObject *Form1::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Form1( "Form1", &Form1::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Form1::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Form1", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Form1::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Form1", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Form1::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"Connect_Device", 0, 0 };
    static const QUMethod slot_1 = {"Toggle_LED", 0, 0 };
    static const QUMethod slot_2 = {"Get_Push_Button_Status", 0, 0 };
    static const QUMethod slot_3 = {"close", 0, 0 };
    static const QUMethod slot_4 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Connect_Device()", &slot_0, QMetaData::Public },
	{ "Toggle_LED()", &slot_1, QMetaData::Public },
	{ "Get_Push_Button_Status()", &slot_2, QMetaData::Public },
	{ "close()", &slot_3, QMetaData::Public },
	{ "languageChange()", &slot_4, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"Form1", parentObject,
	slot_tbl, 5,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Form1.setMetaObject( metaObj );
    return metaObj;
}

void* Form1::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Form1" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool Form1::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Connect_Device(); break;
    case 1: Toggle_LED(); break;
    case 2: Get_Push_Button_Status(); break;
    case 3: close(); break;
    case 4: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Form1::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool Form1::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool Form1::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
