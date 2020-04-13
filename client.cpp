#include "client.h"

CClient::CClient()
{
  tcpSocket = new QTcpSocket(this);
  in.setDevice(tcpSocket);

  init_var();

  connect(tcpSocket, &QIODevice::readyRead, this, &CClient::readData);
  connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &CClient::displayError);
  connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

CClient::~CClient()
{
  tcpSocket->disconnectFromHost();
}

void CClient::init_var()
{
  bRegister = false;
}

void CClient::socketConnect(bool r, QString host, QString port) {
  QByteArray ba = host.toLocal8Bit();

  bRegister = r;

  tcpSocket->abort();
  tcpSocket->connectToHost(ba.data(), port.toUShort());
}

void CClient::processDatagram(QString data)
{
  QByteArray ba;
  QString m;

  int code, sec, s;
  QString d, t;

  if (data == "login:") {
    if (bRegister)
      tcpSocket->write("new");
    else {
      ba = handle.toLocal8Bit();
      tcpSocket->write(ba.data());
    }
    return;
 }

  if (data == "handle:"){
    ba = handle.toLocal8Bit();
    tcpSocket->write(ba.data());
    return;
  }

  if (data == "password:") {
    sig_message("in");
    ba = password.toLocal8Bit();
    tcpSocket->write(ba.data());
    return;
  }

  if (data == "confirm:") {
    ba = password.toLocal8Bit();
    tcpSocket->write(ba.data());
    return;
  }

  if (data.startsWith("I")) {
    code = data.mid(1,2).toInt();
    switch (code) {
      case PLAYER_UID:
           m.sprintf("playerid: %d", data.mid(4).toUInt());
           sig_message(m);
           emit sig_action(ACTION_CONNECTED, "");
           break; // 01 - User ID
      case PLAYER_CHOOSE_CHAIR:
           emit sig_action(ACTION_CHOOSE_CHAIR, data.mid(4));
           break; // 02 - Choose chair
      case PLAYER_SIT_HERE:
           emit sig_action(ACTION_SIT_CHAIR, data.mid(4));
           break; // 03 - Sit chair
      case PLAYER_STAND:
           emit sig_action(ACTION_STAND_CHAIR, data.mid(4));
           break; // 04 - Stand chair
      case TABLE_CREATED:
           emit sig_action(ACTION_CREATE_TABLE, data.mid(4));
           break; // 05 - Create table
      case TABLE_DELETED:
           emit sig_action(ACTION_DELETE_TABLE, data.mid(4));
           break; // 06 - Delete table
      case TABLE_SAY:
           s = data.mid(4).indexOf(' ');                   // extract the name
           m = data.mid(4, s) + tr(" says") + data.mid(4 + s); // extract the message
           sig_message(m);
           break; // 07 - Table say
      case TABLE_YOUR_CARDS:
           emit sig_action(ACTION_SET_CARDS, data.mid(4));
           break; // 08 - Game cards
      case TABLE_YOUR_TURN:
           emit sig_action(ACTION_YOUR_TURN, data.mid(4));
           break; // 09 - Your turn
 //   case SERVER_WHO:
 //        break; // 10 - Who
      case TABLE_LEAVE:
           emit sig_action(ACTION_LEAVE_TABLE, "");
           break; // 11 - Leave
      case TABLE_CARDS_RECEIVED:
           emit sig_action(ACTION_RECEIVE_CARDS, data.mid(4));
           break; // 12 - Received cards
      case TABLE_FORCE_PASS:
           emit sig_action(ACTION_FORCE_PASS, data.mid(4));
           break; // 13 - Game force pass
//    case TABLE_FORCE_PLAY:
//         break; // 14
      case TABLE_PLAY_TWO_CLUBS:
           emit sig_message(tr("You must play the two of clubs."));
           break; // 15 - Play the 2 of clubs
      case TABLE_PLAY:
           emit sig_action(ACTION_PLAY, data.mid(4));
           break; // 16 - Game play a card
      case TABLE_HAND_SCORE:
           emit sig_action(ACTION_HAND_SCORE, data.mid(4));
           break; // 17 - Game hand score
      case TABLE_SCORE:
           emit sig_action(ACTION_SCORE, data.mid(4));
           break; // 18 - Game score
      case TABLE_WHO_AM_I:
           emit sig_action(ACTION_MY_CHAIR, data.mid(4));
           break; // 19 - Game my chair
      case TABLE_NO_TRICK_BONUS:
           emit sig_action(ACTION_BONUS_NO_TRICKS, data.mid(4));
           break; // 20 - Table no trick bonus
      case TABLE_OMNIBUS:
           emit sig_action(ACTION_BONUS_OMNIBUS, data.mid(4));
           break; // 21 - Table omnibus bonus
      case TABLE_PERFECT_100:
           emit sig_action(ACTION_PERFECT_100, data.mid(4));
           break; // 22 - Table perfect 100
      case TABLE_SHOOT_MOON:
            emit sig_action(ACTION_SHOOT_MOON, data.mid(4));
           break; // 23 - Table shoot moon
      case COMMAND_SET:
           sig_message(tr("Command set: "));
           break; // 24 - Command Set
      case TABLE_GAMEOVER:
           sig_action(ACTION_GAMEOVER, data.mid(4));
           break; // 25 - Table Game Over
      case TABLE_PAUSED:
           sig_message(tr("The table is paused"));
           break; // 26 - Table is paused
      case TABLE_UNPAUSED:
           sig_message(tr("The table is unpaused"));
           break; // 27 - Table is unpaused
      case SERVER_ANNOUNCEMENT:
           emit sig_action(ACTION_ANNOUNCEMENT, data.mid(4));
           break; // 28 - Server announcement
      case PLAYER_MOON:
           emit sig_action(ACTION_PLAYER_MOON, data.mid(4));
           break;
    }
  } else if (data.startsWith("E")) {
    code = data.mid(1,2).toInt();
    switch (code) {
      case HANDLE_IS_ILLEGAL: break; // 01
      case HANDLE_UNAVAILABLE:
           m = tr("The Handle '") + handle + tr("' is already registered!");
           sig_message(m);
           break; // 02 - Handle already registered.
      case HANDLE_RESERVED:
           m = tr("The Handle '") + handle + tr("' is reserved.");
           sig_message(m);
           break; // 03 - Reserved handle.
      case WRONG_PASSWORD:
           sig_message(tr("Wrong password!"));
           break; // 04 - Wrong password.
      case HANDLE_TOO_SHORT:
           m = tr("The Handle '") + handle + tr("' is too short. It should contains atlease 3 characters.");
           sig_message(m);
           break; // 05 - Handle too short
      case HANDLE_TOO_LONG:
           m = tr("The Handle '") + handle + tr("' is too long. It should contains a maximum of 8 characters.");
           sig_message(m);
           break; // 06 - Handle too long
      case HANDLE_ILLEGAL_CHAR:
           m = tr("The Handle '") + handle + tr("' constains illegal characters. Only alphabetic, numeric '_' and '-' is accepted.");
           sig_message(m);
           break; // 07 - Handle is illegal
      case PASSWORD_TOO_SHORT:
           sig_message(tr("Your password is too short. It should contains atlease 4 characters."));
           break; // 08 - Password is too short
      case PASSWORD_TOO_LONG:
           sig_message(tr("Your password is too long. It should contains a maximum of 16 characters."));
           break; // 09 - Password is too long
      case PASSWORD_DONT_MATCH:
           sig_message(tr("Your password doesn't match."));
           break; // 10 - Password don't match
      case HANDLE_NOT_REGISTERED:
           m = tr("The Handle '") + handle + tr("' is not registered.");
           sig_message(m);
           break; // 11 - Handle is not registered
      case AUTO_LOGOUT_IDLENESS:
           sec = data.mid(4).toInt();
           if (sec < 60)
             d = tr(" second");
           else if (sec >= 3600) {
                  d = tr(" hour");
                  sec /= 3600;
                }
                else {
                  sec /= 60;
                  d = tr(" minute");
                 }
           m = tr("**** Auto-logout because you were idle more than ") + QString::number(sec) + d + ((sec > 1) ? "s ****" : " ****");

           sig_message(m);
           break; // 12 - Auto-logout due to idleness
      case UNKNOWN_COMMAND:
           sig_message(tr("Unknown command sent to the server."));
           break; // 13 - Unknown command
      case SERVER_SHUTOFF:
           sig_message(tr("The server is shuting down soon for maintenance."));
           break; // 14 - Server switch to shutoff mode
      case SERVER_SHUTDOWN:
           sig_message(tr("The server is shuting down now."));
           break; // 15 - Server shutdown
      case PLAYER_RECONNECT:
           sig_message(tr("This connection is lost due to a reconnection."));
           break; // 16 - Player is reconnecting
      case PLAYER_LOAD_FAILED:
           sig_message(tr("The server failed to load your player."));
           break; // 17 - Player load failed
      case SOCKET_FLOOD:
           sig_message(tr("The server detected flood. Disconnecting."));
           break; // 18 - Socket flood
      case PLAYER_AT_TABLE:
           sig_message(tr("You are already at a table."));
           break; // 19 - Player is at a table
      case PLAYER_NO_TABLE:
           sig_message(tr("You didn't join any tables."));
           break; // 20 - Player no table
      case TABLE_WRONG_CHAIR:
           sig_message(tr("You can't sit on that chair. It's already used."));
           break; // 21 - Wrong chair
      case PLAYER_SIT_DELAY:
           sig_message(tr("Wait delay. Switching from chair to chair."));
           break; // 22 - Wait sit delay
      case SOCKET_MAX_CONN_IP:
           sig_message(tr("Too many connection from this ip."));
           break; // 23 - Too many connection from your ip
      case TABLE_NOT_FOUND:
           m = tr("Table ") + data.mid(4) + tr(" not found.");
           sig_message(m);
           break; // 24 - Table not found
      case AMBIGOUS_COMMAND:
           m = tr("The command is ambigous: ") + data.mid(4);
           sig_message(m);
           break; // 25 - Ambigous command
      case TABLE_MUTED:
           sig_message(tr("The table is muted."));
           break; // 26 - Table is muted
      case PLAYER_NOT_SAT:
           sig_message(tr("You needs to sit to perform this command."));
           break; // 27 - Player is not sat
      case TABLE_NOT_3PASSED:
           sig_message(tr("You needs to pass 3 cards."));
           break; // 28 - Needs to pass 3 cards
      case TABLE_ALREADY_PASSED:
           sig_message(tr("The cards has already been passed."));
           break; // 29 - The cards has already been passed
      case TABLE_ILLEGAL_CARD:
           sig_message(tr("Illegal card."));
           break; // 30 - You passed an illegal card
      case TABLE_CARD_NOT_FOUND:
           sig_message(tr("Card not found."));
           break; // 31 - Card not found
      case TABLE_DOUBLE_CARD:
           sig_message(tr("Same card passed twice"));
           break; // 32 - Double card
      case TABLE_PASSING_OVER:
           sig_message(tr("Passing card is over. We are now playing."));
           break; // 33 - Passing time is over
      case TABLE_STARTED:
           sig_message(tr("That game has already started."));
           break; // 34 - The game has started
      case TABLE_FULL:
           sig_message(tr("Can't join the table is full."));
           break; // 35 - The table is full
      case TABLE_NOT_STARTED:
           sig_message(tr("The game hasn't started yet."));
           break; // 36 - Table not started
      case TABLE_NOT_YOUR_TURN:
           sig_message(tr("It's not your turn."));
           break; // 37 - It's not your turn
      case TABLE_CANT_BREAK_HEART:
           sig_message(tr("You can't break heart yet."));
           break; // 38 - Can't break heart
      case TABLE_QUEEN_SPADE:
           sig_message(tr("You can't play the queen of spade."));
           break; // 39 - Queen spade too early
      case TABLE_WRONG_SUIT:
           sig_message(tr("Wrong suit."));
           break; // 40 - Wrong suit
      case SOCKET_MAX_REGISTER_IP:
           sig_message(tr("You can't register. The maximum registration from your ip has been reached."));
           break; // 41 - Max IP registration
      case TABLE_ALREADY_PLAYED:
           sig_message(tr("You already played."));
           break; // 42 - Already played
      case SOCKET_ILLEGAL_INPUT:
           sig_message(tr("Socket illegal input."));
           break; // 43 - Socket illegal input
      case PLAYER_NOT_MOON:
           sig_message(tr("You did not shoot the moon."));
           break; // 44 - Player did not moon
      case COMMAND_WRONG_PARAMETER:
           sig_message(tr("Wrong parameters"));
           break; // 45 - Command wrong parameter
    }
  }

  tcpSocket->waitForBytesWritten();
}

void CClient::readData() {
 QByteArray array = tcpSocket->read(tcpSocket->bytesAvailable());
 QByteArray::iterator iter = array.begin();

 QString s_data;

 while(iter != array.end()) {
    QChar c = *iter;

    if (c == '\0') {
      emit sig_message(s_data);
      processDatagram(s_data);
      s_data.clear();
    } else
        s_data.append(c.toLatin1());
    iter++;
 }
}

void CClient::socketDisconnected() {
  emit sig_action(ACTION_DISCONNECTED, "");
}

void CClient::send(QString mesg) {
  QByteArray out;
  out += mesg;

  if (tcpSocket->state() == QAbstractSocket::ConnectedState) {
    tcpSocket->write(out);
    tcpSocket->waitForBytesWritten(1000);
  }
}

void CClient::displayError(QAbstractSocket::SocketError socketError)
{
  switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
           emit sig_message (tr("The host was not found. Please check the host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
           emit sig_message (tr("The connection was refused by the peer. "
                             "Make sure the host name and port "
                             "settings are correct."));
        break;
    default:
          emit sig_message(tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
    }
}

void CClient::setHandle(QString h)
{
  handle = h;
}

void CClient::setPassword(QString p)
{
  password = p;
}
