#include <UI/MainWindow.hpp>
#include <ACO/sample_tree.hpp>

MainWindow::MainWindow(QWidget *parent)
: 
QMainWindow(parent)
{
    std::stringstream inputStream;
    inputStream.str(sample_tree);
    std::shared_ptr<tree_nodes> mapNodes(new tree_nodes);
    mapNodes->parse(inputStream);
    

    createMenu();
    createMap();
    createControls();
    createStatusBar();
    setWindowTitle(tr("ACO - mass transit lines"));
    QWidget * mainWidget = new QWidget(this);


    QHBoxLayout *mainLayout = new QHBoxLayout;
    setMenuBar(_menuBar);
    mainLayout->addWidget(_mapGroupBox);
    mainLayout->addWidget(_controlGroupBox);

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setGeometry(100, 100, 600, 500);

    connectWidgets();
    reSetAco();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenu()
{
    _menuBar = new QMenuBar;

    _fileMenu = new QMenu(tr("&File"), this);
    _loadMapAction = _fileMenu->addAction(tr("L&oad map"), this, SLOT(selectFile()));
    _loadExampleMapAction = _fileMenu->addAction(tr("Load example m&ap"), this, SLOT(defaultFile()));
    _menuBar->addMenu(_fileMenu);
}

void MainWindow::createMap()
{
    _mapGroupBox = new QGroupBox(tr("Map"));
    QHBoxLayout *layout = new QHBoxLayout;

    _mapWidget = new MapWidget(_mapGroupBox);
    layout->addWidget(_mapWidget);
    _mapGroupBox->setLayout(layout);
}

void MainWindow::connectWidgets()
{
    connect(_setAcoButton, &QPushButton::clicked, this, &MainWindow::getAcoSetting);
    connect(this,  &MainWindow::newMap, _mapWidget, &MapWidget::setMap);
    connect(this, &MainWindow::newMap, this, &MainWindow::setMap);
    
    connect(this,  &MainWindow::newMap, _mapWidget, &MapWidget::reSetAco);
    connect(this, &MainWindow::newMap, this, &MainWindow::reSetAco);
    connect(_mapWidget,  &MapWidget::mapUpdated, _mapWidget, &MapWidget::reSetAco);
    connect(_mapWidget, &MapWidget::mapUpdated, this, &MainWindow::reSetAco);

    connect(this,  &MainWindow::newAco, _mapWidget, &MapWidget::setAco);
    connect(this, &MainWindow::newAco, this, &MainWindow::setAco);

    connect(_runAcoButton, &QPushButton::clicked, this, &MainWindow::runAco);
    connect(_singleStepAcoButton, &QPushButton::clicked, this, &MainWindow::stepAco);
    connect(this, &MainWindow::resultSet, this, &MainWindow::setResults);
    connect(this, &MainWindow::resultSet, _mapWidget, &MapWidget::setResults);

    connect(_feromoneInput, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MainWindow::blockRun);
    connect(_populationInput, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::blockRun);
    connect(_alphaInput, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MainWindow::blockRun);
    connect(_evaporationInput, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MainWindow::blockRun);
    connect(_feromoneWeightInput, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MainWindow::blockRun);
    connect(_costWeightInput, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &MainWindow::blockRun);
}

void MainWindow::createControls()
{
    _controlGroupBox = new QGroupBox(tr("Controls"));
    QGridLayout *layout = new QGridLayout;

    _acoSettingsGroupBox = new QGroupBox(tr("Ant System settings"));

    QFormLayout *settingsLayout = new QFormLayout;

    _feromoneSettingLabel = new QLabel(tr("Feromone per cycle:"));
    _populationSettingLabel = new QLabel(tr("Population:"));
    _alphaSettingLabel = new QLabel(tr("Initial feromone:"));
    _evaporationSettingLabel = new QLabel(tr("Evaporation rate:"));
    _feromoneWeightSettingLabel = new QLabel(tr("Effect of feromone:"));
    _costWeightSettingLabel = new QLabel(tr("Effect of path cost:"));

    _feromoneInput = new QDoubleSpinBox();
    _feromoneInput->setMaximum(10);
    _feromoneInput->setMinimum(0);
    _feromoneInput->setValue(1);
    
    _populationInput = new QSpinBox();
    _populationInput->setMaximum(1000);
    _populationInput->setMinimum(1);
    _populationInput->setValue(10);

    _alphaInput = new QDoubleSpinBox();
    _alphaInput->setMaximum(10);
    _alphaInput->setMinimum(0.01);
    _alphaInput->setValue(1);

    _evaporationInput = new QDoubleSpinBox();
    _evaporationInput->setMaximum(1);
    _evaporationInput->setMinimum(0.01);
    _evaporationInput->setSingleStep(0.01);
    _evaporationInput->setValue(0.3);

    _feromoneWeightInput = new QDoubleSpinBox();
    _feromoneWeightInput->setMaximum(10);
    _feromoneWeightInput->setMinimum(0.01);
    _feromoneWeightInput->setValue(1);
    _costWeightInput = new QDoubleSpinBox();
    _costWeightInput->setMaximum(10);
    _costWeightInput->setMinimum(0.01);
    _costWeightInput->setValue(1);

    _setAcoButton = new QPushButton(tr("Set"));
    _resetDefaultAcoButton = new QPushButton(tr("Reset to default"));

    settingsLayout->addRow(_feromoneSettingLabel, _feromoneInput);
    settingsLayout->addRow(_populationSettingLabel, _populationInput);
    settingsLayout->addRow(_alphaSettingLabel, _alphaInput);
    settingsLayout->addRow(_evaporationSettingLabel, _evaporationInput);
    settingsLayout->addRow(_feromoneWeightSettingLabel, _feromoneWeightInput);
    settingsLayout->addRow(_costWeightSettingLabel, _costWeightInput);
    settingsLayout->addRow(new QWidget, _setAcoButton);

    _acoSettingsGroupBox->setLayout(settingsLayout);

    layout->addWidget(_acoSettingsGroupBox, 0, 0);

    _acoControlGroupBox = new QGroupBox(tr("Run"));

    QGridLayout * controlLayout = new QGridLayout();

    _numberOfIterationsSettingLabel = new QLabel(tr("Number of iterations"));
    _numberOfIterationsInput = new QSpinBox();
    _numberOfIterationsInput->setMinimum(20);
    _numberOfIterationsInput->setMaximum(2000);
    _runAcoButton = new QPushButton(tr("Run"));
    _singleStepAcoButton = new QPushButton(tr("Do one step"));

    controlLayout->addWidget(_numberOfIterationsSettingLabel, 0, 0);
    controlLayout->addWidget(_numberOfIterationsInput, 0, 1);
    controlLayout->addWidget(_runAcoButton, 1, 0);
    controlLayout->addWidget(_singleStepAcoButton, 1, 1);

    _acoControlGroupBox->setLayout(controlLayout);
    
    layout->addWidget(_acoControlGroupBox, 0, 1);


    _acoResultsGroupBox = new QGroupBox(tr("Results"));
    QHBoxLayout * resultLayout = new QHBoxLayout();

    _resultTabs = new QTabWidget(this);


    _resultOrderTable = new QTableWidget;
    _resultFeromoneTable = new QTableWidget;
    _resultCostTable = new QTableWidget;

    _resultTabs->addTab(_resultOrderTable, "Order");
    _resultTabs->addTab(_resultFeromoneTable, "Feromone matrix");
    _resultTabs->addTab(_resultCostTable, "Cost matrix");

    resultLayout->addWidget(_resultTabs);
    _acoResultsGroupBox->setLayout(resultLayout);

    layout->addWidget(_acoResultsGroupBox, 1, 0, 1, 2);
    _controlGroupBox->setLayout(layout);


}
void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::selectFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, 
        tr("Open File")
    );
    if(!fileName.isNull())
    {
        std::shared_ptr<std::string> fileNamePtr(new std::string(fileName.toUtf8().constData()));
        setFile(fileNamePtr);
    }
}
void MainWindow::defaultFile()
{
    std::shared_ptr<std::stringstream> stream(new std::stringstream);
    stream->str(sample_tree);
    setFileStream(stream);
}
void MainWindow::setFile(std::shared_ptr<std::string> file)
{
    std::shared_ptr<std::ifstream> fileStream(new std::ifstream);
    fileStream->open(file->data());
    setFileStream(fileStream);
    fileStream->close();
}
void MainWindow::setFileStream(std::shared_ptr<std::istream> stream)
{
    std::shared_ptr<tree_nodes> map(new tree_nodes);
    map->parse(*stream);
    emit newMap(map);
}
void MainWindow::setMap(const std::shared_ptr<tree_nodes> map)
{
    _mapNodes = map;
}
void MainWindow::setResults(int index)
{
    _acoResultsGroupBox->setDisabled(false);
    _resultIndex = index;

    if(_aco->T.size() != 0)
    {
        _resultOrderTable->setRowCount(1);
        _resultOrderTable->setColumnCount(_aco->T.back().size());
        const std::vector<int> & best(_aco->T.back());
        for (size_t i = 0; i < best.size(); i++)
        {
            QTableWidgetItem *tWidget = new QTableWidgetItem;
            _resultOrderTable->setItem(0, i, tWidget);
            tWidget->setData(Qt::DisplayRole, best[i]);
            _resultOrderTable->setColumnWidth(i, 30);
        }
    }
    

    _resultFeromoneTable->setRowCount(_aco->nodes.stride);
    _resultFeromoneTable->setColumnCount(_aco->nodes.stride);
    _resultCostTable->setRowCount(_aco->nodes.stride);
    _resultCostTable->setColumnCount(_aco->nodes.stride);

    for (size_t i = 0; i < _aco->nodes.stride; i++)
    {
        _resultFeromoneTable->setColumnWidth(i, 60);
        _resultCostTable->setColumnWidth(i, 60);

        for (size_t j = 0; j < _aco->nodes.stride; j++)
        {
            QTableWidgetItem *tWidget = new QTableWidgetItem;
            tWidget->setData(Qt::DisplayRole, round( _aco->nodes.feromoneMatrix[i*_aco->nodes.stride + j] * 1000.0 ) / 1000.0);
            _resultFeromoneTable->setItem(i, j, tWidget);

            tWidget = new QTableWidgetItem;
            tWidget->setData(Qt::DisplayRole, round( _aco->nodes.costMatrix[i*_aco->nodes.stride + j] * 10.0 ) / 10.0);
            _resultCostTable->setItem(i, j, tWidget);
        }
    }
}
void MainWindow::setAco(const std::shared_ptr<aco_as> as)
{
    _aco = as;
    _acoControlGroupBox->setDisabled(false);
}
void MainWindow::blockRun()
{
    _acoControlGroupBox->setDisabled(true);
    _acoResultsGroupBox->setDisabled(true);
}
void MainWindow::getAcoSetting()
{
    if(_mapNodes != nullptr)
    {
        _mapNodes->calculateMarkedCostMatrix();
        std::shared_ptr<aco_as> as(new aco_as(
            _feromoneInput->value(),
            _populationInput->value(),
            _alphaInput->value(),
            _evaporationInput->value(),
            _feromoneWeightInput->value(),
            _costWeightInput->value(),
            _mapNodes->markedCostMatrix
        ));
        emit newAco(as);
    }
}
void MainWindow::reSetAco()
{
    _aco.reset();
    _acoControlGroupBox->setDisabled(true);
    _acoResultsGroupBox->setDisabled(true);
}
void MainWindow::runAco()
{
    int no = _numberOfIterationsInput->value();
    for (size_t i = 0; i < no; i++)
    {
        statusBar()->showMessage(tr("Runnning: ") + QString::number((i*100)/no) + "%");
        stepAco();
    }
    statusBar()->showMessage(tr("Done"));
}

void MainWindow::stepAco()
{
    _aco->doCycle();
    emit resultSet(_aco->T.size() - 1);
}