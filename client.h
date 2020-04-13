#ifndef CLIENT_H
#define CLIENT_H

#include <Qt>
#include <QWidget>
#include <QTcpSocket>
#include <QDataStream>
#include <QtNetwork>

#define ACTION_CONNECTED        1
#define ACTION_CHOOSE_CHAIR     2
#define ACTION_SIT_CHAIR        3
#define ACTION_STAND_CHAIR      4
#define ACTION_CREATE_TABLE     5
#define ACTION_DELETE_TABLE     6
#define ACTION_DISCONNECTED     7
#define ACTION_SET_CARDS        8
#define ACTION_FORCE_PASS       9
#define ACTION_RECEIVE_CARDS   10
#define ACTION_YOUR_TURN       11
#define ACTION_PLAY            12
#define ACTION_HAND_SCORE      13
#define ACTION_SCORE           14
#define ACTION_MY_CHAIR        15
#define ACTION_SHOOT_MOON      16
#define ACTION_BONUS_OMNIBUS   17
#define ACTION_BONUS_NO_TRICKS 18
#define ACTION_PERFECT_100     19
#define ACTION_GAMEOVER        20
#define ACTION_ANNOUNCEMENT    21
#define ACTION_PLAYER_MOON     22
#define ACTION_LEAVE_TABLE     23

#define HANDLE_IS_ILLEGAL     1
#define HANDLE_UNAVAILABLE    2
#define HANDLE_RESERVED       3
#define WRONG_PASSWORD        4
#define HANDLE_TOO_SHORT      5
#define HANDLE_TOO_LONG       6
#define HANDLE_ILLEGAL_CHAR   7
#define PASSWORD_TOO_SHORT    8
#define PASSWORD_TOO_LONG     9
#define PASSWORD_DONT_MATCH   10
#define HANDLE_NOT_REGISTERED 11
#define AUTO_LOGOUT_IDLENESS  12
#define UNKNOWN_COMMAND       13
#define SERVER_SHUTOFF        14
#define SERVER_SHUTDOWN       15
#define PLAYER_RECONNECT      16
#define PLAYER_LOAD_FAILED    17
#define SOCKET_FLOOD          18
#define PLAYER_AT_TABLE       19
#define PLAYER_NO_TABLE       20
#define TABLE_WRONG_CHAIR     21
#define PLAYER_SIT_DELAY      22
#define SOCKET_MAX_CONN_IP    23
#define TABLE_NOT_FOUND       24
#define AMBIGOUS_COMMAND      25
#define TABLE_MUTED           26
#define PLAYER_NOT_SAT        27
#define TABLE_NOT_3PASSED     28
#define TABLE_ALREADY_PASSED  29
#define TABLE_ILLEGAL_CARD    30
#define TABLE_CARD_NOT_FOUND  31
#define TABLE_DOUBLE_CARD     32
#define TABLE_PASSING_OVER    33
#define TABLE_STARTED         34
#define TABLE_FULL            35
#define TABLE_NOT_STARTED     36
#define TABLE_NOT_YOUR_TURN   37
#define TABLE_CANT_BREAK_HEART 38
#define TABLE_QUEEN_SPADE     39
#define TABLE_WRONG_SUIT      40
#define SOCKET_MAX_REGISTER_IP 41
#define TABLE_ALREADY_PLAYED  42
#define SOCKET_ILLEGAL_INPUT  43
#define PLAYER_NOT_MOON       44
#define COMMAND_WRONG_PARAMETER 45

#define PLAYER_UID            1
#define PLAYER_CHOOSE_CHAIR   2
#define PLAYER_SIT_HERE       3
#define PLAYER_STAND          4
#define TABLE_CREATED         5
#define TABLE_DELETED         6
#define TABLE_SAY             7
#define TABLE_YOUR_CARDS      8
#define TABLE_YOUR_TURN       9
#define SERVER_WHO            10
#define TABLE_LEAVE           11
#define TABLE_CARDS_RECEIVED  12
#define TABLE_FORCE_PASS      13
#define TABLE_FORCE_PLAY      14
#define TABLE_PLAY_TWO_CLUBS  15
#define TABLE_PLAY            16
#define TABLE_HAND_SCORE      17
#define TABLE_SCORE           18
#define TABLE_WHO_AM_I        19
#define TABLE_NO_TRICK_BONUS  20
#define TABLE_OMNIBUS         21
#define TABLE_PERFECT_100     22
#define TABLE_SHOOT_MOON      23
#define COMMAND_SET           24
#define TABLE_GAMEOVER        25
#define TABLE_PAUSED          26
#define TABLE_UNPAUSED        27
#define SERVER_ANNOUNCEMENT   28
#define PLAYER_MOON           29

class CClient : public QWidget
{
    Q_OBJECT

public:
    CClient();
    ~CClient();

private:
    QTcpSocket *tcpSocket;
    QDataStream in;
    QNetworkSession *networkSession;
    bool bRegister;
    QString handle;
    QString password;

    void init_var();
    void processDatagram(QString data);

signals:
    void sig_message(QString mesg);
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
