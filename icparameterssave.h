#ifndef ICPARAMETERSSAVE_H
#define ICPARAMETERSSAVE_H

#include <QSettings>
#include <QString>
#include <QLocale>
#include <QTranslator>
#include <QDate>
#ifndef Q_WS_WIN32
#include <fcntl.h>
#include <sys/ioctl.h>
#endif

class ICParametersSave : public QSettings
{
    Q_OBJECT
public:

    enum OperationLevel
    {
        MachineOperator,
        MachineAdmin,
        AdvanceAdmin
    };
    static ICParametersSave * Instance()
    {
        if(instance_ == NULL)
        {
            instance_ = new ICParametersSave("sysconfig/Multi-axisManipulatorSystem.ini");
        }
        return instance_;
    }
    ~ICParametersSave();

    void SaveParameter(const QString & group, const QString & key, const QVariant & value, bool issync = true);
    QVariant GetParameter(const QString & group, const QString & key, const QVariant & defaultValue);

    void SetCommunicationConfig(const QString& device,
                                int baudRate,
                                const QString& parity,
                                int dataBit,
                                int stopBit);
    QString Device(){ return GetParameter(CommunicationConfig, "Device", "/dev/ttyS0").toString();}
    int BaudRate(){ return GetParameter(CommunicationConfig, "BaudRate", 57600).toInt();}
    QString Parity() { return GetParameter(CommunicationConfig, "Parity", "none").toString();}
    int DataBit() { return GetParameter(CommunicationConfig, "DataBit", 8).toInt();}
    int StopBit(){ return GetParameter(CommunicationConfig, "StopBit", 1).toInt();}

    QString MoldName(const QString & defaultMoldName) { return GetParameter("BootParameter", "moldName", defaultMoldName).toString(); }
    void SetMoldName(const QString & moldName) { SaveParameter("BootParameter", "moldName", moldName); }

    int RestTime(const int defaultHour_){return GetParameter("BootParameter", "hour_", defaultHour_).toInt(); }
    void SetRestTime(const int & hour_){SaveParameter("BootParameter", "hour_", hour_); }

    QLocale::Language Language(){ return static_cast<QLocale::Language>(GetParameter(SystemLocale, "SystemLanguage", QLocale::Chinese).toInt());}
    void SetLanguage(QLocale::Language language, bool isSync  = true);
    void LoadInitLocale() { SetCountry(Country());}
    QLocale::Country Country() { return static_cast<QLocale::Country>(GetParameter(SystemLocale, "SystemCountry", QLocale::China).toInt());}
    void SetCountry(QLocale::Country country, bool isSync = true);

    QTranslator* Translator() { return translator_;}

    bool KeyTone() { return GetParameter(ProductConfig, "KeyTone", true).toBool();}
#ifndef Q_WS_WIN32
    void SetKeyTone(bool isOn, bool isSave = true)  {SaveParameter(ProductConfig, "KeyTone", isOn, isSave);ioctl(beepFD_, 0, isOn ? 1 : 0);}
#else
    void SetKeyTone(bool isOn, bool isSave = true)  {SaveParameter(ProductConfig, "KeyTone", isOn,isSave);}
#endif

    bool VerifyPassword(OperationLevel level, const QString& password);
    void SetPassword(OperationLevel level, const QString& password);

    double DistanceRotation(const QString& axisName);/* {return GetParameter("AxisConfig", axisName, 0).toDouble();}*/
    void SetDistanceRotation(const QString& axisName, double value);/* { SaveParameter("AxisConfig", axisName, value);}*/
    //    void SetFileName(const QString & fileName);
    bool IsExtentFunctionUsed() {return GetParameter(SystemMachine, "AdminSettings", false).toBool();}
    void SetExtentFunctionUsed(bool isUsed, bool isSync = true) {SaveParameter(SystemMachine, "AdminSettings", isUsed, isSync);}

    bool IsLimitMaxSpeedOn() { return GetParameter(SystemMachine, "MaxSpeed", false).toBool();}
    void LimitMaxSpeed(bool isUsed, bool isSync = true) {SaveParameter(SystemMachine, "MaxSpeed", isUsed, isSync);}

    uint BackLightTime() { return GetParameter(ProductConfig, "BackLight", 5).toUInt();}
    void SetBackLightTime(uint time) { SaveParameter(ProductConfig, "BackLight", time);}

    uint Brightness() { return GetParameter(ProductConfig, "Brightness", 8).toUInt();}
    void SetBrightness(uint brightness, bool isSync = true);

    bool IsSingleArm() { return GetParameter(SystemMachine, "ArmNum", false).toBool();}
    void SetSingleArm(bool isSingle) { SaveParameter(SystemMachine, "ArmNum", isSingle);}

    bool IsRegisterFunctinOn() { return GetParameter(SystemMachine, "RegisterFunction", false).toBool();}
    void SetRegisterFunction(bool ison) { SaveParameter(SystemMachine, "RegisterFunction", ison);}
    
    bool IsAdjustFunctionOn() { return GetParameter(SystemMachine, "AdjustFunction", false).toBool();}
    void SetAdjustFunction(bool ison) { SaveParameter(SystemMachine, "AdjustFunction", ison);}

    QString SuperPassward() { return GetParameter(SystemMachine, "SuperPassward","123456").toString();}
    void SetSuperPassward(const QString &passward) { SaveParameter(SystemMachine, "SuperPassward", passward);}

    QString Client() { return GetParameter(SystemMachine, "Client", "").toString();}
    void SetClient(const QString &client) { SaveParameter(SystemMachine, "Client", client);}

    QString ShipmentDate() { return GetParameter(SystemMachine, "ShipmentDate", "").toString();}
    void SetShipmentDate(const QString &date)  { SaveParameter(SystemMachine, "ShipmentDate", date);}

    bool IsProductSave() { return GetParameter(SystemMachine, "IsProductSave", false).toBool();}
    void SetProductSave(bool isSave) { SaveParameter(SystemMachine, "IsProductSave", isSave);}

    QString FacotryCode() { return GetParameter(SystemMachine, "FacotryCode", "000000").toString();}
    void SetFacotryCode(const QString& code) { SaveParameter(SystemMachine, "FacotryCode", code);}

    QDateTime BootDatetime() { return QDateTime::fromString(GetParameter(SystemMachine, "BootDateTime", QDateTime::currentDateTime().toString()).toString());}
      void SetBootDatetime(QDateTime dt) { SaveParameter(SystemMachine, "BootDateTime", dt.toString(), false);}

    int CareCycle(int item) { return GetParameter(SystemMachine, QString("CareCycleItem-%1").arg(item), 30).toInt();}
    void SetCareCycle(int item, int cycle) { SaveParameter(SystemMachine, QString("CareCycleItem-%1").arg(item), cycle);}

    QDate NextCycle(int item) { return GetParameter(SystemMachine, QString("NextCycleItem-%1").arg(item), QDate::currentDate().addDays(30)).toDate();}
    void SetNextCycle(int item, QDate th) { SaveParameter(SystemMachine, QString("NextCycleItem-%1").arg(item), th);}

    bool IsSuperLogin() { return isSuperLogin_;}

signals:
    void CurrentLanguageChanged();

public:
    const QString ProductOperationer;
    const QString ProductAdministrator;
    const QString ProductAlarmHistory;
    const QString SystemMachine;
    const QString SystemLocale;
    const QString InstructMultidotPut;
    const QString CommunicationConfig;
    const QString ProductConfig;

private:
    static ICParametersSave* instance_;
    QTranslator *translator_;
    QTranslator *sysTranslator_;
    QMap<QString, double> axisToRotate_;
    int beepFD_;
    bool isSuperLogin_;
    ICParametersSave(const QString fileName = QString());
};



#endif // ICPARAMETERSSAVE_H
