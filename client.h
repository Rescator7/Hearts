#ifdef ONLINE_PLAY

#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QDataStream>
#include <QtNetwork>

#define STATUS_PASSING              0
#define STATUS_PASSED               1
#define STATUS_PLAYING              2
#define STATUS_YOUR_TURN            3
#define STATUS_WAITING              4

#define ACTION_CONNECTED            1
#define ACTION_CHOOSE_CHAIR         2
#define ACTION_SIT_CHAIR            3
#define ACTION_STAND_CHAIR          4
#define ACTION_CREATE_TABLE         5
#define ACTION_DELETE_TABLE         6
#define ACTION_DISCONNECTED         7
#define ACTION_SET_CARDS            8
#define ACTION_FORCE_PASS           9
#define ACTION_RECEIVE_CARDS        10
#define ACTION_YOUR_TURN            11
#define ACTION_PLAY                 12
#define ACTION_HAND_SCORE           13
#define ACTION_SCORE                14
#define ACTION_MY_CHAIR             15
#define ACTION_SHOOT_MOON           16
#define ACTION_BONUS_OMNIBUS        17
#define ACTION_BONUS_NO_TRICKS      18
#define ACTION_PERFECT_100          19
#define ACTION_GAMEOVER             20
#define ACTION_ANNOUNCEMENT         21
#define ACTION_PLAYER_MOON          22
#define ACTION_LEAVE_TABLE          23
#define ACTION_CLEAR_TABLE          24
#define ACTION_TABLE_MUTED          25
#define ACTION_SHUFFLE              26
#define ACTION_STATS                27
#define ACTION_GAME_STARTED         28
#define ACTION_RECONNECTED          29
#define ACTION_WRONG_VALUE          30
#define ACTION_TIME_BANK            31

// Users Datagrams Errors
#define DGE_HANDLE_IS_ILLEGAL       1
#define DGE_HANDLE_UNAVAILABLE      2
#define DGE_HANDLE_RESERVED         3
#define DGE_WRONG_PASSWORD          4
#define DGE_HANDLE_TOO_SHORT        5
#define DGE_HANDLE_TOO_LONG         6
#define DGE_HANDLE_ILLEGAL_CHAR     7
#define DGE_PASSWORD_TOO_SHORT      8
#define DGE_PASSWORD_TOO_LONG       9
#define DGE_PASSWORD_DONT_MATCH     10
#define DGE_HANDLE_NOT_REGISTERED   11
#define DGE_AUTO_LOGOUT_IDLENESS    12
#define DGE_UNKNOWN_COMMAND         13
#define DGE_SERVER_SHUTOFF          14
#define DGE_SERVER_SHUTDOWN         15
#define DGE_PLAYER_RECONNECT        16
#define DGE_PLAYER_LOAD_FAILED      17
#define DGE_SOCKET_FLOOD            18
#define DGE_PLAYER_AT_TABLE         19
#define DGE_PLAYER_NO_TABLE         20
#define DGE_TABLE_WRONG_CHAIR       21
#define DGE_PLAYER_SIT_DELAY        22
#define DGE_SOCKET_MAX_CONN_IP      23
#define DGE_TABLE_NOT_FOUND         24
#define DGE_AMBIGOUS_COMMAND        25
#define DGE_TABLE_MUTED             26
#define DGE_PLAYER_NOT_SAT          27
#define DGE_TABLE_NOT_3PASSED       28
#define DGE_TABLE_ALREADY_PASSED    29
#define DGE_TABLE_ILLEGAL_CARD      30
#define DGE_TABLE_CARD_NOT_FOUND    31
#define DGE_TABLE_DOUBLE_CARD       32
#define DGE_TABLE_PASSING_OVER      33
#define DGE_TABLE_STARTED           34
#define DGE_TABLE_FULL              35
#define DGE_TABLE_NOT_STARTED       36
#define DGE_TABLE_NOT_YOUR_TURN     37
#define DGE_TABLE_CANT_BREAK_HEART  38
#define DGE_TABLE_QUEEN_SPADE       39
#define DGE_TABLE_WRONG_SUIT        40
#define DGE_SOCKET_MAX_REGISTER_IP  41
#define DGE_TABLE_ALREADY_PLAYED    42
#define DGE_SOCKET_ILLEGAL_INPUT    43
#define DGE_PLAYER_NOT_MOON         44
#define DGE_COMMAND_WRONG_PARAMETER 45
#define DGE_ADMIN_NOT_FOUND         46
#define DGE_ADMIN_ABOVE             47
#define DGE_TABLE_CORRUPTED         48
#define DGE_WRONG_VALUE             49
#define DGE_TABLE_NOT_OWNER         50

// Users datagrams Informations
#define DGI_PLAYER_UID              1
#define DGI_PLAYER_CHOOSE_CHAIR     2
#define DGI_PLAYER_SIT_HERE         3
#define DGI_PLAYER_STAND            4
#define DGI_TABLE_CREATED           5
#define DGI_TABLE_DELETED           6
#define DGI_TABLE_SAY               7
#define DGI_TABLE_YOUR_CARDS        8
#define DGI_TABLE_YOUR_TURN         9
//#define DGI_SERVER_WHO            10
#define DGI_TABLE_LEAVE             11
#define DGI_TABLE_CARDS_RECEIVED    12
#define DGI_TABLE_FORCE_PASS        13
#define DGI_TABLE_FORCE_PLAY        14
#define DGI_TABLE_PLAY_TWO_CLUBS    15
#define DGI_TABLE_PLAY              16
#define DGI_TABLE_HAND_SCORE        17
#define DGI_TABLE_SCORE             18
#define DGI_TABLE_WHO_AM_I          19
#define DGI_TABLE_NO_TRICK_BONUS    20
#define DGI_TABLE_OMNIBUS           21
#define DGI_TABLE_PERFECT_100       22
#define DGI_TABLE_SHOOT_MOON        23
#define DGI_COMMAND_SET             24
#define DGI_TABLE_GAMEOVER          25
#define DGI_TABLE_PAUSED            26
#define DGI_TABLE_UNPAUSED          27
#define DGI_SERVER_ANNOUNCEMENT     28
#define DGI_PLAYER_MOON             29
#define DGI_TABLE_CLEAR             30
#define DGI_ADMIN_SET               31
#define DGI_TABLE_SHUFFLE           32
#define DGI_GAME_STATS              33
#define DGI_NEW_PASSWORD            34
#define DGI_GAME_STARTED            35
#define DGI_TEXT                    36
#define DGI_RECONNECTED             37
#define DGI_TIME_BANK               38

// Users Datagrams help commands informations
#define DGI_HELP_DATE               1
#define DGI_HELP_EXIT               2
#define DGI_HELP_HELP               3
#define DGI_HELP_JOIN               4
#define DGI_HELP_LEAVE              5
#define DGI_HELP_MOON               6
#define DGI_HELP_MUTE               7
#define DGI_HELP_NEW                8
#define DGI_HELP_PASS               9
#define DGI_HELP_PASSWORD           10
#define DGI_HELP_PLAY               11
#define DGI_HELP_SAY                12
#define DGI_HELP_SIT                13
#define DGI_HELP_STATS              14
#define DGI_HELP_TABLES             15
#define DGI_HELP_UPTIME             16
#define DGI_HELP_WHO                17
#define DGI_HELP_START              18

// Admin Datagrams help commands informations
#define DGI_HELP_ADMIN              50
#define DGI_HELP_ANNOUNCE           51
#define DGI_HELP_PAUSE              52
#define DGI_HELP_SET                53
#define DGI_HELP_SHUTDOWN           54
#define DGI_HELP_SHUTOFF            55
#define DGI_HELP_TEST               56

class CClient : public QWidget
{
    Q_OBJECT

public:
    CClient();
    ~CClient();

private:
    QTcpSocket *tcpSocket;
    QDataStream in;
    bool bRegister;
    QString handle;
    QString password;

    void init_var();
    void processDatagram(QString data);

signals:
    void sig_message(QString mesg);
    void sig_error(QString mesg);
    void sig_action(unsigned int action, QString param);

private slots:
    void displayError(QAbstractSocket::SocketError socketError);
    void socketDisconnected();

public:
    void readData();
    void socketConnect(bool r, QString host, QString port);
    void send(QString mesg);
    void setHandle(QString h);
    void setPassword(QString p);
};
#endif // CLIENT_H

#endif // ONLINE_PLAY
