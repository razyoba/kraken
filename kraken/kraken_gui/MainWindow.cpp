#include <QFileDialog>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  _ui(new Ui::MainWindow)
{
  _ui->setupUi( this );

  _codeModel = NULL;
  _selectionModel = NULL;

  // making statistics block look neat
  _ui->statisticsText->viewport()->setAutoFillBackground( false );
  _ui->statisticsText->setBackgroundRole( QPalette::Window );
  _ui->statisticsText->viewport()->setMouseTracking( true );
  _ui->statisticsText->viewport()->setCursor( Qt::ArrowCursor );

  connect( _ui->actionLoad, SIGNAL( triggered() ), SLOT( loadFile() ) );
  connect( _ui->actionShowProgramListing, SIGNAL( triggered() ), SLOT( showProgramListing() ) );
}

MainWindow::~MainWindow()
{
  delete _ui;
}

void MainWindow::loadFile()
{
  QString pathToFile = QFileDialog::getOpenFileName( this, tr( "Load executable" ), "", tr( "Executable files (*.exe)" ) );

  if ( _codeModel != NULL )
  {
    disconnect( _selectionModel, SIGNAL( selectionChanged( QItemSelection,QItemSelection ) ), this, SLOT( showItemIndexes() ) );

    _ui->listView->setSelectionModel( NULL );
    _ui->listView->setModel( NULL );

    delete _selectionModel;
    delete _codeModel;
  }

  _codeModel = new CodeModel( pathToFile, this );
  _selectionModel = new QItemSelectionModel( _codeModel, this );

  _ui->listView->setModel( _codeModel );
  _ui->listView->setSelectionModel( _selectionModel );

  connect( _selectionModel, SIGNAL( selectionChanged( QItemSelection, QItemSelection ) ), SLOT( showItemIndexes() ) );

  _ui->statisticsText->clear();
  _ui->statisticsText->appendPlainText( QString( "Chunk count: %1\nInstruction count: %2" ).arg( _codeModel->getCodeBlockCount() ).arg( _codeModel->getCodeBlockItemCount() ) );

  _ui->actionShowProgramListing->setEnabled( true );
}

void MainWindow::showProgramListing()
{
  _codeModel->showProgramListing();
}

void MainWindow::showItemIndexes()
{

}
