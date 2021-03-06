
# include  " ui_DuSqliteManagerMainWindow.h "
# include  " DuSqliteManagerFindReplaceDialog.h "
# include  " DuSqliteManagerPreferencesDialog.h "
# include  " DuSqliteManagerNewTableDialog.h "
# include  " DuSqliteManagerEditDataDialog.h "
# include  " DuSqliteManagerMainWindow.h "
# include  " DuSqliteManagerSettings.h "
# include  < QUrl >
# include  < QFile >
# include  < QPrinter >
# include  < QSqlError >
# include  < QSqlQuery >
# include  < QMimeData >
# include  < QDropEvent >
# include  < QTextStream >
# include  < QMessageBox >
# include  < QFileDialog >
# include  < QPrintDialog >
# include  < QSqlQueryModel >
# include  < QDragMoveEvent >
# include  < QTreeWidgetItem >
# include  < QDragLeaveEvent >
# include  < QDragEnterEvent >

en utilisant DuListItem = QListWidgetItem;

namespace  DuarteCorporation
{
DuSqliteManagerMainWindow :: DuSqliteManagerMainWindow (parent QWidget *):
    QMainWindow (parent),
    ui ( nouveau Ui :: MainWindow)
{
    ui-> setupUi ( this );
    mDb = QSqlDatabase :: addDatabase ( " QSQLITE " );
    mQueryModel = new  QSqlQueryModel ( this );
    UI-> tableView -> setModel ( mQueryModel );
    centralWidget () -> layout () -> setMargin ( 0 );
    auto vfont = DuSqliteManagerSettingsLoad (GFONT, KFONTTYPE,
                                             ui-> textEdit -> font ());
    ui-> textEdit -> setFont ( valeur de vfont <QFont> ());
    setAcceptDrops ( true );
    connexions ();
}

DuSqliteManagerMainWindow :: ~ DuSqliteManagerMainWindow ()
{
    supprimer ui;
}

void  DuSqliteManagerMainWindow :: dragEnterEvent (QDragEnterEvent * e)
{
    e-> accept ();
}

void  DuSqliteManagerMainWindow :: dragLeaveEvent (QDragLeaveEvent * e)
{
    e-> accept ();
}

void  DuSqliteManagerMainWindow :: dragMoveEvent (QDragMoveEvent * e)
{
    e-> accept ();
}

void  DuSqliteManagerMainWindow :: dropEvent (QDropEvent * e)
{
    données automatiques = e-> mimeData ();
    urls auto = data-> urls ();
    pour ( auto u: urls) {
        nom de l' auto = u. toLocalFile ();
        auto baseitem = nouveau DuTreeItem;
        baseitem-> setIcon ( 0 , QIcon ( " : base " ));
        baseitem-> setText ( 0 , nom);
        mDb . setDatabaseName (nom);
        if (! mDb . open ()) {
            QMessageBox :: critical ( this , " Erreur " , QString ( " Erreur: Pas d'erreur " \
                                                         " Pourrait ouvrir " \
                                  " la Base de données:% 1 " ). arg (nom));
            return ;
        }
        QSqlQuery qry;
        qry. exec ( " SELECT tbl_name FROM sqlite_master WHERE type LIKE 'table' " );
        while (qry, next ()) {
            auto tableitem = nouveau DuTreeItem;
            tableitem-> setIcon ( 0 , QIcon ( " : table " ));
            tableitem-> setText ( 0 ., qry valeur ( 0 ). toString ());
            baseitem-> addChild (tableau);
        }
        ui-> treeWidget -> addTopLevelItem (baseitem);
    }
}

void  DuSqliteManagerMainWindow :: on_actionAbrir_base_triggered ()
{
    nom automatique = QFileDialog :: getOpenFileName ( this , " Base de données ouverte " ,
                                               QDir :: currentPath (),
                                               " Base de données(* .db) ;; " \
                                               " N'importe quel type(*. *) " );
    if ( nom.isEmpty ())
        return ;
    auto baseitem = nouveau DuTreeItem;
    baseitem-> setIcon ( 0 , QIcon ( " : base " ));
    baseitem-> setText ( 0 , nom);
    mDb . setDatabaseName (nom);
    if (! mDb . open ()) {
        QMessageBox :: critical ( this , " Erreur " , QString ( " Erreur: Pas d'erreur " \
                                                     " Podido Abrir " \
                              " la base de données:% 1 " ). arg (nom));
        return ;
    }
    QSqlQuery qry;
    qry. exec ( " SELECT tbl_name FROM sqlite_master WHERE type LIKE 'table' " );
    while (qry, next ()) {
        auto tableitem = nouveau DuTreeItem;
        tableitem-> setIcon ( 0 , QIcon ( " : tabla " ));
        tableitem-> setText ( 0 ., qry valeur ( 0 ). toString ());
        baseitem-> addChild (tableau);
    }
    ui-> treeWidget -> addTopLevelItem (baseitem);
}

void  DuSqliteManagerMainWindow :: on_actionGuardar_base_triggered ()
{
    auto name = QFileDialog :: getSaveFileName ( ceci , " Sauvegarder la base " ,
                                               QDir :: currentPath (),
                                               " Base de données (* .db) ;; " \
                                               " N'importe quel type (*. *) " );
    if ( nom.isEmpty ())
        return ;
    auto baseitem = nouveau DuTreeItem;
    baseitem-> setIcon ( 0 , QIcon ( " : base " ));
    baseitem-> setText ( 0 , nom);
    ui-> treeWidget -> addTopLevelItem (baseitem);
    mDb . setDatabaseName (nom);
    if (! mDb . open ()) {
        QMessageBox :: critical ( this , " Erreur " , QString ( " Erreur: Pas d'erreur " \
                                                     " Pourrait sauver " \
                              "la base de données:% 1 " ). arg (nom));
    }
}

void  DuSqliteManagerMainWindow :: on_actionAbrir_SQL_triggered ()
{
    nom automatique = QFileDialog :: getOpenFileName ( this , " Ouvrir SQL " ,
                                               QDir :: currentPath (),
                                               " Archivo SQL (* .sql) ;; " \
                                               "N'importe quel type (*. *) " );
    if ( nom.isEmpty ())
        retour ;
    QFichier F (nom);
    if (! F open (QIODevice :: Text | QIODevice :: ReadOnly)) {
        QMessageBox :: critical ( this , " Erreur " , QString ( " Erreur:% 1 " ).
                              arg (F. errorString ()));
        return ;
    }
    QTextStream S (& F);
    ui-> textEdit -> setText (S. readAll ());
    F. fermer ();
}

void  DuSqliteManagerMainWindow :: on_actionGuardar_SQL_triggered ()
{
    auto name = QFileDialog :: getSaveFileName ( ceci , " sauvegarder SQL " ,
                                               QDir :: currentPath (),
                                               " Archives SQL (* .sql) ;; " \
                                               " N'importe quel type (*. *) " );
    if ( nom.isEmpty ())
        return ;
    QFichier F (nom);
    if (! F open (QIODevice :: Text | QIODevice :: WriteOnly)) {
        QMessageBox :: critical ( this , " Erreur " , QString ( " Erreur:% 1 " ).
                              arg (F. errorString ()));
        return ;
    }
    QTextStream S (& F);
    S << ui-> textEdit -> toPlainText ();
    F. flush ();
    F. fermer ();
}

void  DuSqliteManagerMainWindow :: on_actionImprimir_triggered ()
{
    QPrintDialog P ( ceci );
    if (P. exec () == QDialog :: Rejected) {
        return ;
    }
    ui-> textEdit -> impression (P. printer ());
}

void  DuSqliteManagerMainWindow :: on_actionQuitar_triggered ()
{
    fermer ();
}

void  DuSqliteManagerMainWindow :: on_actionCopiar_triggered ()
{
    ui-> textEdit -> copy ();
}

void  DuSqliteManagerMainWindow :: on_actionCortar_triggered ()
{
    ui-> textEdit -> couper ();
}

void  DuSqliteManagerMainWindow :: on_actionPegar_triggered ()
{
    ui-> textEdit -> paste ();
}

void  DuSqliteManagerMainWindow :: on_actionEjecutar_triggered ()
{
    auto sql = ui-> textEdit -> toPlainText ();
    if (sql. startsWith ( " select " , Qt :: CaseInsensitive)) {
        mQueryModel -> setQuery (sql);
    } else  if (sql. startsWith ( " créer une table " , Qt :: CaseInsensitive)) {
        QSqlQuery qry;
        if (! qry. exec (sql)) {
            ui-> listWidget -> addItem ( nouveau  DuListItem ( QIcon ( " : échec " ), sql));
            retour ;
        }
        ui-> listWidget -> addItem ( nouveau  DuListItem ( QIcon ( " : correct " ), sql));
        base automatique = mDb . databaseName ();
        int i, taille = ui-> treeWidget -> topLevelItemCount ();
        pour (i = 0 ; i <taille; ++ i) {
            if (ui-> treeWidget -> topLevelItem (i) -> texte ( 0 ). toLower ()
                    == base. toLower ()) {
                rompre ;
            }
        }
        auto baseitem = ui-> treeWidget -> topLevelItem (i);
        auto createtable = QString ( " créer une table " );
        if (sql contient ( " s'il n'existe pas " , Qt :: CaseInsensitive)) {
            createtable = QString ( " créer une table s'il n'existe pas " );
        }
        int pos1 = créable. taille ();
        int pos2 = sql. indexOf ( " ( " , pos1);
        table automatique = sql. mid (pos1, pos2-pos1);
        table = table. mid ( 0 , tableau indexOf ( "  " ));
        size = baseitem-> childCount ();
        pour (i = 0 ; i <taille; ++ i) {
            if (baseitem-> enfant (i) -> texte ( 0 ). toLower () == table. toLower ()) {
                return ;
            }
        }
        auto tableitem = nouveau DuTreeItem;
        tableitem-> setIcon ( 0 , QIcon ( " : table " ));
        tableitem-> setText ( 0 , table);
        baseitem-> addChild (tablaitem);
    } else  if (sql. startsWith ( " drop table " , Qt :: CaseInsensitive)) {
        QSqlQuery qry;
        if (! qry. exec (sql)) {
            ui-> listWidget -> addItem ( nouveau  DuListItem ( QIcon ( " : échec " ), sql));
            return ;
        }
        ui-> listWidget -> addItem ( nouveau  DuListItem ( QIcon ( " : correct " ), sql));
        base automatique = mDb . databaseName ();
        int i, tam = ui-> treeWidget -> topLevelItemCount ();
        pour (i = 0 ; i <tam; ++ i) {
            if (ui-> treeWidget -> topLevelItem (i) -> texte ( 0 ). toLower ()
                    == base. toLower ()) {
                rompre ;
            }
        }
        auto baseitem = ui-> treeWidget -> topLevelItem (i);
        tam = baseitem-> childCount ();
        auto droptable = QString ( " drop table " );
        table automatique = sql. mid (droptable. size ());
        table = table. mon ( 1 );
        pour (i = 0 ; i <tam; ++ i) {
            if (baseitem-> enfant (i) -> texte ( 0 ). toLower () == table.Low ()) {
                rompre ;
            }
        }
        baseitem-> removeChild (baseitem-> enfant (i));
    } else {
        QSqlQuery qry;
        if (! qry. exec (sql)) {
            ui-> listWidget -> addItem ( nouveau  DuListItem ( QIcon ( " : échec " ), sql));
            return ;
        }
        ui-> listWidget -> addItem ( nouveau  DuListItem ( QIcon ( " : correct " ), sql));
    }
}

void  DuSqliteManagerMainWindow :: on_actionPreferencias_triggered ()
{
    DuSqliteManagerPreferencesDialog p ( this );
    connect (& p, & DuSqliteManagerPreferencesDialog :: fontSelected,
            [&] ( const QFont & f) {
        ui-> textEdit -> setFont (f);
        DuSqliteManagerSettingsSave (GFONT, KFONTTYPE, f);
        // duSave (GFONT, KFONTSIZE, f.pointSizeF ());
    });
    p. exec ();
}

void  DuSqliteManagerMainWindow :: on_actionManual_triggered ()
{
    // A FAIRE: Cela reste
}



void  DuSqliteManagerMainWindow :: on_actionBuscar_y_reemplazar_triggered ()
{
    DuSqliteManagerFindReplaceDialog F ( ceci );
    F. setTextEdit (ui-> textEdit );
    F. exec ();
}

void  DuSqliteManagerMainWindow :: on_treeWidget_clicked ( const QModelIndex & index)
{
    si ( index . parent (). isValid ()) {
        return ;
    }
    nom automatique = index . données (). toString ();
    mDb . setDatabaseName (nom);
    if (! mDb . open ()) {
        QMessageBox :: critical ( this , " Erreur " , QString ( " Erreur: Pas d'erreur " \
                                                     " Pourrait ouvrir " \
                              "la base de données:% 1 " ). arg (nom));
    }
}

void  DuSqliteManagerMainWindow :: connections ()
{
    connect (ui-> treeWidget , & DuSqliteManagerTreeWidget :: newTable, [&] () {
        if (ui-> treeWidget -> type () == DuSqliteManagerTreeWidget :: BASE) {
            DuSqliteManagerNewTableDialog D ( this );
            if (D. exec () == QDialog :: Rejeté) {
                return ;
            }
            // ajouter le code SQL pour la table
            ui-> textEdit -> setPlainText ( résultat de D. ());
        } else {
            QMessageBox :: warning ( this , "Avertissement" ,
                           "Cette fonction est seulement autorisée" \
                                  "quand une base de données est sélectionnée");
        }
    });

    connect (ui-> treeWidget , & DuSqliteManagerTreeWidget :: dropTable, [&] () {
        if (ui-> treeWidget -> type () == DuSqliteManagerTreeWidget :: TABLE) {
            ui-> textEdit -> setPlainText ( QString ( " DROP TABLE% 1 " ).
                   arg (ui-> treeWidget -> currentItem () -> text ( 0 )));
        } else {
            QMessageBox :: warning ( this , " Avertissement " ,
                              "Cette fonction est seulement autorisée" \
                              "quand une table est sélectionnée" );
            return ;
        }
    });

    connect (ui-> treeWidget , & DuSqliteManagerTreeWidget :: removeBase, [&] () {
        if (ui-> treeWidget -> type () == DuSqliteManagerTreeWidget :: BASE) {
            supprimer ui-> treeWidget -> currentItem ();
        } else {
            QMessageBox :: warning ( this , " Avertissement " ,
                                 " Cette fonction est seulement autorisée" \
                                 " quand une base de données est sélectionnée " );
        }
    });

    connect (ui-> treeWidget , & DuSqliteManagerTreeWidget :: selectFrom, [&] () {
        if (ui-> treeWidget -> type () == DuSqliteManagerTreeWidget :: TABLE) {
            ui-> textEdit -> setPlainText ( QString ( " SELECT * FROM% 1 " ).
                                       arg (ui-> treeWidget -> currentItem ()
                                           -> text ( 0 )));
        } else {
            QMessageBox :: warning ( this , " Avertissement " ,
                                 " Cette fonction est seulement autorisée " \
                                 "quand une table est sélectionnée" );
        }
    });

    connect (ui-> treeWidget , & DuSqliteManagerTreeWidget :: editData, [&] () {
        if (ui-> treeWidget -> type () == DuSqliteManagerTreeWidget :: TABLE) {
            DuSqliteManagerEditDataDialog D (ui-> treeWidget -> currentItem ()
                                            -> texte ( 0 ), this );
            D. exec ();
        } else {
            QMessageBox :: warning ( this , " Avertissement " ,
                                 "Cette fonction est seulement autorisée" \
                                 " quand une table est sélectionnée " );
        }
    });
}
} // fin de l'espace
