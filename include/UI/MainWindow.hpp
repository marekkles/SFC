#ifndef MainWindow_hpp
#define MainWindow_hpp

#include <QMainWindow>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QAction>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QTextEdit>
#include <QStatusBar>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QSlider>
#include <QTableWidget>

#include <UI/MapWidget.hpp>
#include <ACO/tree_nodes.hpp>

#include <ACO/aco_as.hpp>

#include <fstream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void newFile(std::shared_ptr<std::string> as);
    void newFileStream(std::shared_ptr<std::istream> stream);
    void newMap(const std::shared_ptr<tree_nodes> map);
    void newAco(const std::shared_ptr<aco_as> as);
    void resultSet(int index);
public slots:
    void selectFile();
    void defaultFile();
    void setFile(std::shared_ptr<std::string> file);
    void setFileStream(std::shared_ptr<std::istream> stream);
    void setMap(const std::shared_ptr<tree_nodes> map);
    void setResults(int index);
    void setAco(const std::shared_ptr<aco_as> as);
    void blockRun();
    void getAcoSetting();
    void reSetAco();
    void runAco();
    void stepAco();
private:
    void connectWidgets();
    void createMenu();
    void createMap();
    void createControls();
    void createStatusBar();
    
    int _resultIndex;
    std::shared_ptr<tree_nodes> _mapNodes;
    std::shared_ptr<aco_as> _aco;

    QMenuBar * _menuBar;
    QGroupBox * _controlGroupBox;
    QGroupBox * _acoSettingsGroupBox;
    QGroupBox * _acoControlGroupBox;
    QGroupBox * _acoResultsGroupBox;
    QGroupBox * _mapGroupBox;
    MapWidget * _mapWidget;
    QMenu * _fileMenu;
    QAction * _loadMapAction;
    QAction * _loadExampleMapAction;
    QAction * _exitAction;

    QLabel * _feromoneSettingLabel;
    QLabel * _populationSettingLabel;
    QLabel * _alphaSettingLabel;
    QLabel * _evaporationSettingLabel;
    QLabel * _feromoneWeightSettingLabel;
    QLabel * _costWeightSettingLabel;
    
    QDoubleSpinBox * _feromoneInput;
    QSpinBox * _populationInput;
    QDoubleSpinBox * _alphaInput;
    QDoubleSpinBox * _evaporationInput;
    QDoubleSpinBox * _feromoneWeightInput;
    QDoubleSpinBox * _costWeightInput;
    QPushButton * _setAcoButton;
    QPushButton * _resetDefaultAcoButton;

    QLabel * _numberOfIterationsSettingLabel;
    QSpinBox * _numberOfIterationsInput;
    QPushButton * _runAcoButton;
    QPushButton * _singleStepAcoButton;
    
    QLabel * _resultOrderTableLabel;

    QTabWidget *_resultTabs;
    QTableWidget * _resultOrderTable;
    QTableWidget * _resultFeromoneTable;
    QTableWidget * _resultCostTable;

};

#endif 
