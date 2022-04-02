#ifdef ONLINE_PLAY

#include "client.h"
#include "define.h"

CClient::CClient()
{
  tcpSocket = new QTcpSocket(this);
  in.setDevice(tcpSocket);

  init_var();

  connect(tcpSocket, &QIODevice::readyRead, this, &CClient::readData);

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
  connect(tcpSocket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
#else
  connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
#endif // QT_VERSION

  connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));
}

CClient::~CClient()
{
  tcpSocket->disconnectFromHost();

  delete tcpSocket;
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
    tcpSocket->waitForBytesWritten();
    return;
 }

  if (data == "handle:"){
    ba = handle.toLocal8Bit();
    tcpSocket->write(ba.data());
    tcpSocket->waitForBytesWritten();
    return;
  }

  if (data == "password:") {
    ba = password.toLocal8Bit();
    tcpSocket->write(ba.data());
    tcpSocket->waitForBytesWritten();
    return;
  }

  if (data == "confirm:") {
    ba = password.toLocal8Bit();
    tcpSocket->write(ba.data());
    tcpSocket->waitForBytesWritten();
    return;
  }

#ifdef DEBUG_DATAGRAMS
  sig_message(data);
#endif

  if (data.startsWith("I")) {
    code = data.mid(1,2).toInt();
    switch (code) {
      case DGI_PLAYER_UID:
           emit sig_action(ACTION_CONNECTED, "");
           break; // 01 - User ID
      case DGI_PLAYER_CHOOSE_CHAIR:
           emit sig_action(ACTION_CHOOSE_CHAIR, data.mid(4));
           break; // 02 - Choose chair
      case DGI_PLAYER_SIT_HERE:
           emit sig_action(ACTION_SIT_CHAIR, data.mid(4));
           break; // 03 - Sit chair
      case DGI_PLAYER_STAND:
           emit sig_action(ACTION_STAND_CHAIR, data.mid(4));
           break; // 04 - Stand chair
      case DGI_TABLE_CREATED:
           emit sig_action(ACTION_CREATE_TABLE, data.mid(4));
           break; // 05 - Create table
      case DGI_TABLE_DELETED:
           emit sig_action(ACTION_DELETE_TABLE, data.mid(4));
           break; // 06 - Delete table
      case DGI_TABLE_SAY:
           s = data.mid(4).indexOf(' ');                   // extract the name
           m = data.mid(4, s) + tr(" says") + data.mid(4 + s); // extract the message
           sig_message(m);
           break; // 07 - Table say
      case DGI_TABLE_YOUR_CARDS:
           emit sig_action(ACTION_SET_CARDS, data.mid(4));
           break; // 08 - Game cards
      case DGI_TABLE_YOUR_TURN:
           emit sig_action(ACTION_YOUR_TURN, data.mid(4));
           break; // 09 - Your turn
//      case DGI_SERVER_WHO:
//           emit sig_message(tr("Who: "));
//           emit sig_message(data.mid(4));
//           break; // 10 - Who
      case DGI_TABLE_LEAVE:
           emit sig_action(ACTION_LEAVE_TABLE, "");
           break; // 11 - Leave
      case DGI_TABLE_CARDS_RECEIVED:
           emit sig_action(ACTION_RECEIVE_CARDS, data.mid(4));
           break; // 12 - Received cards
      case DGI_TABLE_FORCE_PASS:
           emit sig_action(ACTION_FORCE_PASS, data.mid(4));
           break; // 13 - Game force pass
//    case DGI_TABLE_FORCE_PLAY:
//         break; // 14
      case DGI_TABLE_PLAY_TWO_CLUBS:
           emit sig_error(tr("You must play the two of clubs."));
           break; // 15 - Play the 2 of clubs
      case DGI_TABLE_PLAY:
           emit sig_action(ACTION_PLAY, data.mid(4));
           break; // 16 - Game play a card
      case DGI_TABLE_HAND_SCORE:
           emit sig_action(ACTION_HAND_SCORE, data.mid(4));
           break; // 17 - Game hand score
      case DGI_TABLE_SCORE:
           emit sig_action(ACTION_SCORE, data.mid(4));
           break; // 18 - Game score
      case DGI_TABLE_WHO_AM_I:
           emit sig_action(ACTION_MY_CHAIR, data.mid(4));
           break; // 19 - Game my chair
      case DGI_TABLE_NO_TRICK_BONUS:
           emit sig_action(ACTION_BONUS_NO_TRICKS, data.mid(4));
           break; // 20 - Table no trick bonus
      case DGI_TABLE_OMNIBUS:
           emit sig_action(ACTION_BONUS_OMNIBUS, data.mid(4));
           break; // 21 - Table omnibus bonus
      case DGI_TABLE_PERFECT_100:
           emit sig_action(ACTION_PERFECT_100, data.mid(4));
           break; // 22 - Table perfect 100
      case DGI_TABLE_SHOOT_MOON:
           emit sig_action(ACTION_SHOOT_MOON, data.mid(4));
           break; // 23 - Table shoot moon
      case DGI_COMMAND_SET:
           sig_message(tr("Command set: "));
           sig_message(data.mid(4));
           break; // 24 - Command Set
      case DGI_TABLE_GAMEOVER:
           sig_action(ACTION_GAMEOVER, data.mid(4));
           break; // 25 - Table Game Over
      case DGI_TABLE_PAUSED:
           sig_message(tr("The table is paused"));
           break; // 26 - Table is paused
      case DGI_TABLE_UNPAUSED:
           sig_message(tr("The table is unpaused"));
           break; // 27 - Table is unpaused
      case DGI_SERVER_ANNOUNCEMENT:
           emit sig_action(ACTION_ANNOUNCEMENT, data.mid(4));
           break; // 28 - Server announcement
      case DGI_PLAYER_MOON:
           emit sig_action(ACTION_PLAYER_MOON, data.mid(4));
           break; // 29 - Player moon
      case DGI_TABLE_CLEAR:
           emit sig_action(ACTION_CLEAR_TABLE, "");
           break; // 30 - Table clear
      case DGI_ADMIN_SET:
           m = tr("The user '") + data.mid(4) + tr("' has been set to level admin!");
           sig_message(m);
           break; // 31 - Admin set
      case DGI_TABLE_SHUFFLE:
           sig_action(ACTION_SHUFFLE, "");
           break; // 32 - Table shuffle
      case DGI_GAME_STATS:
           sig_action(ACTION_STATS, data.mid(4));
           break; // 33 - Game stats
      case DGI_NEW_PASSWORD:
           sig_message(tr("New password has been set."));
           break; // 34 - New password
      case DGI_GAME_STARTED:
           sig_action(ACTION_GAME_STARTED, data.mid(4));
           break; // 35 - Game started
      case DGI_TEXT:
           sig_message(data.mid(4));
           break; // 36 - Text
      case DGI_RECONNECTED:
           sig_action(ACTION_RECONNECTED, data.mid(4));
           break; // 37 - Reconnected
      case DGI_TIME_BANK:
           sig_action(ACTION_TIME_BANK, data.mid(4));
           break; // 38 - Time Bank
    }
    return;
  }

  if (data.startsWith("E")) {
    code = data.mid(1,2).toInt();
    switch (code) {
      case DGE_HANDLE_IS_ILLEGAL: break; // 01
      case DGE_HANDLE_UNAVAILABLE:
           m = tr("The Handle '") + handle + tr("' is already registered!");
           sig_error(m);
           break; // 02 - Handle already registered.
      case DGE_HANDLE_RESERVED:
           m = tr("The Handle '") + handle + tr("' is reserved.");
           sig_error(m);
           break; // 03 - Reserved handle.
      case DGE_WRONG_PASSWORD:
           sig_error(tr("Wrong password!"));
           break; // 04 - Wrong password.
      case DGE_HANDLE_TOO_SHORT:
           m = tr("The Handle '") + handle + tr("' is too short. It should contains atlease 3 characters.");
           sig_error(m);
           break; // 05 - Handle too short
      case DGE_HANDLE_TOO_LONG:
           m = tr("The Handle '") + handle + tr("' is too long. It should contains a maximum of 8 characters.");
           sig_error(m);
           break; // 06 - Handle too long
      case DGE_HANDLE_ILLEGAL_CHAR:
           m = tr("The Handle '") + handle + tr("' constains illegal characters. Only alphabetic, numeric '_' and '-' is accepted.");
           sig_error(m);
           break; // 07 - Handle is illegal
      case DGE_PASSWORD_TOO_SHORT:
           sig_error(tr("Your password is too short. It should contains atlease 4 characters."));
           break; // 08 - Password is too short
      case DGE_PASSWORD_TOO_LONG:
           sig_error(tr("Your password is too long. It should contains a maximum of 16 characters."));
           break; // 09 - Password is too long
      case DGE_PASSWORD_DONT_MATCH:
           sig_error(tr("Your password doesn't match."));
           break; // 10 - Password don't match
      case DGE_HANDLE_NOT_REGISTERED:
           m = tr("The Handle '") + handle + tr("' is not registered.");
           sig_error(m);
           break; // 11 - Handle is not registeredl
      case DGE_AUTO_LOGOUT_IDLENESS:
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
      case DGE_UNKNOWN_COMMAND:
           sig_error(tr("Unknown command or help file."));
           break; // 13 - Unknown command
      case DGE_SERVER_SHUTOFF:
           sig_message(tr("The server is shuting down soon for maintenance."));
           break; // 14 - Server switch to shutoff mode
      case DGE_SERVER_SHUTDOWN:
           sig_message(tr("The server is shuting down now."));
           break; // 15 - Server shutdown
      case DGE_PLAYER_RECONNECT:
           sig_message(tr("This connection is lost due to a reconnection."));
           break; // 16 - Player is reconnecting
      case DGE_PLAYER_LOAD_FAILED:
           sig_message(tr("The server failed to load your player."));
           break; // 17 - Player load failed
      case DGE_SOCKET_FLOOD:
           sig_message(tr("The server detected flood. Disconnecting."));
           break; // 18 - Socket flood
      case DGE_PLAYER_AT_TABLE:
           sig_error(tr("You are already at a table."));
           break; // 19 - Player is at a table
      case DGE_PLAYER_NO_TABLE:
           sig_error(tr("You didn't join any tables."));
           break; // 20 - Player no table
      case DGE_TABLE_WRONG_CHAIR:
           sig_error(tr("You can't sit on that chair. It's already used."));
           break; // 21 - Wrong chair
      case DGE_PLAYER_SIT_DELAY:
           sig_error(tr("Wait delay. Switching from chair to chair."));
           break; // 22 - Wait sit delay
      case DGE_SOCKET_MAX_CONN_IP:
           sig_error(tr("Too many connection from this ip."));
           break; // 23 - Too many connection from your ip
      case DGE_TABLE_NOT_FOUND:
           m = tr("Table ") + data.mid(4) + tr(" not found.");
           sig_error(m);
           break; // 24 - Table not found
      case DGE_AMBIGOUS_COMMAND:
           m = tr("The command or help matches: ") + data.mid(4);
           sig_error(m);
           break; // 25 - Ambigous command
      case DGE_TABLE_MUTED:
           sig_action(ACTION_TABLE_MUTED, tr("The table is muted."));
           break; // 26 - Table is muted
      case DGE_PLAYER_NOT_SAT:
           sig_error(tr("You needs to sit to perform this command."));
           break; // 27 - Player is not sat
      case DGE_TABLE_NOT_3PASSED:
           sig_error(tr("You needs to pass 3 cards."));
           break; // 28 - Needs to pass 3 cards
      case DGE_TABLE_ALREADY_PASSED:
           sig_error(tr("The cards has already been passed."));
           break; // 29 - The cards has already been passed
      case DGE_TABLE_ILLEGAL_CARD:
           sig_error(tr("Illegal card."));
           break; // 30 - You passed an illegal card
      case DGE_TABLE_CARD_NOT_FOUND:
           sig_error(tr("Card not found."));
           break; // 31 - Card not found
      case DGE_TABLE_DOUBLE_CARD:
           sig_error(tr("Same card passed twice"));
           break; // 32 - Double card
      case DGE_TABLE_PASSING_OVER:
           sig_error(tr("Passing card is over. We are now playing."));
           break; // 33 - Passing time is over
      case DGE_TABLE_STARTED:
           sig_error(tr("That game has already started."));
           break; // 34 - The game has started
      case DGE_TABLE_FULL:
           sig_error(tr("Can't join the table is full."));
           break; // 35 - The table is full
      case DGE_TABLE_NOT_STARTED:
           sig_error(tr("The game hasn't started yet."));
           break; // 36 - Table not started
      case DGE_TABLE_NOT_YOUR_TURN:
           sig_error(tr("It's not your turn."));
           break; // 37 - It's not your turn
      case DGE_TABLE_CANT_BREAK_HEART:
           sig_error(tr("You can't break heart yet."));
           break; // 38 - Can't break heart
      case DGE_TABLE_QUEEN_SPADE:
           sig_error(tr("You can't play the queen of spade."));
           break; // 39 - Queen spade too early
      case DGE_TABLE_WRONG_SUIT:
           sig_error(tr("Wrong suit."));
           break; // 40 - Wrong suit
      case DGE_SOCKET_MAX_REGISTER_IP:
           sig_error(tr("You can't register. The maximum registration from your ip has been reached."));
           break; // 41 - Max IP registration
      case DGE_TABLE_ALREADY_PLAYED:
           sig_error(tr("You already played."));
           break; // 42 - Already played
      case DGE_SOCKET_ILLEGAL_INPUT:
           sig_error(tr("Socket illegal input."));
           break; // 43 - Socket illegal input
      case DGE_PLAYER_NOT_MOON:
           sig_error(tr("You did not shoot the moon."));
           break; // 44 - Player did not moon
      case DGE_COMMAND_WRONG_PARAMETER:
           sig_error(tr("Wrong parameters"));
           break; // 45 - Command wrong parameter
      case DGE_ADMIN_NOT_FOUND:
           m = tr("The user '") + data.mid(4) + tr("' has not been found!");
           sig_error(m);
           break; // 46 - Admin not found
      case DGE_ADMIN_ABOVE:
           m = tr("The user '") + data.mid(4) + tr("' is already an admin!");
           sig_message(m);
           break; // 47 - Admin level above
      case DGE_TABLE_CORRUPTED:
           sig_error(tr("The table is corrupted. Game aborted!"));
           break; // 48 - Table corrupted
      case DGE_WRONG_VALUE:
           sig_action(ACTION_WRONG_VALUE, data.mid(4));
           break; // 49 - Wrong value
      case DGE_TABLE_NOT_OWNER:
           sig_error(tr("You are not the owner of the table!"));
           break; // 50 - Table not owner
    }
    return;
  }

  if (data.startsWith("H")) {
    code = data.mid(1,2).toInt();
    switch (code) {
      case DGI_HELP_DATE:
           sig_message(tr("date - Show the current server date."));
           break; // 1 - HELP_DATE
      case DGI_HELP_EXIT:
           sig_message(tr("exit - Exit the server and return to offline playing mode."));
           break; // 2 - HELP_EXIT
      case DGI_HELP_HELP:
           sig_message(tr("help - Show the command list.\r\n"
                          "help <command> - Show <command>'s help.\r\n"));
           break; // 3 - HELP_HELP
      case DGI_HELP_JOIN:
           sig_message(tr("join <id> - Join table #id.\r\n"
                          "join <id> <chair> - Join table #id and sit on chair (n, s, e, w)."));
           break; // 4 - HELP_JOIN
      case DGI_HELP_LEAVE:
           sig_message(tr("leave - Leave a table.\r\n"
                          "Please note: If you were playing that would make you finish in fourth place."));
           break; // 5 - HELP_LEAVE
      case DGI_HELP_MOON:
           sig_message(tr("moon - Used during new moon to choose between add and substract. Usage: \"moon +\"  or \"mooon -\"."));
           break; // 6 - HELP_MOON
      case DGI_HELP_MUTE:
           sig_message(tr("mute - Mute your table. Turn the table in quiet mode play."));
           break; // 7 - HELP_MUTE
      case DGI_HELP_NEW:
           sig_message(tr("new - Create a new table. Usage: New <flags>. Note: Use interface \"create table\"."));
           break; // 8 - HELP_NEW
      case DGI_HELP_PASS:
           sig_message(tr("pass - Pass your 3 cards. Used by the client when you click button pass.\r\n"
                          "Usage: pass pos1 pos2 pos3. Values are card posititions 0-12."));
           break; // 9 - HELP_PASS
      case DGI_HELP_PASSWORD:
           sig_message(tr("password - Change your password. Usage: password old new."));
           break; // 10 - HELP_PASSWORD
      case DGI_HELP_PLAY:
           sig_message(tr("play - Play a card. Usage: play card."));
           break; // 11 - HELP_PLAY
      case DGI_HELP_SAY:
           sig_message(tr("say - Say <text> at your table."));
           break; // 12 - HELP_SAY
      case DGI_HELP_SIT:
           sig_message(tr("sit - Sit on a chair. Usage: Sit <chair>. Values: n,s,w,e"));
           break; // 13 - HELP_SIT
      case DGI_HELP_STATS:
           sig_message(tr("stats - Show your online statistics."));
           break; // 14 - HELP_STATS
      case DGI_HELP_TABLES:
           sig_message(tr("tables - Show the list of tables."));
           break; // 15 - HELP_TABLES
      case DGI_HELP_UPTIME:
           sig_message(tr("uptime - Show the server version and uptime."));
           break; // 16 - HELP_UPTIME
      case DGI_HELP_WHO:
           sig_message(tr("who - Show the list of connected players."));
           break; // 17 - HELP_WHO
      case DGI_HELP_START:
           sig_message(tr("Start - Start the game. The empty chairs will be played by BOT."));
           break; // 18 - HELP_START

      case DGI_HELP_ADMIN:
           sig_message(tr("admin - Make a user to admin level. Usage: Admin user.\r\n"
                          "Note: require level super user."));
           break; // 50 - HELP_ADMIN
      case DGI_HELP_ANNOUNCE:
           sig_message(tr("announce - Make a server announcement. Shown to all players."));
           break; // 51 - HELP_ANNOUNCE
      case DGI_HELP_PAUSE:
           sig_message(tr("pause - Put a table on pause. This must be used for debuging purpose only."));
           break; // 52 - HELP_PAUSE
      case DGI_HELP_SET:
           sig_message(tr("set - Show or set the list of server configuration parameters."));
           break; // 53 - HELP_SET
      case DGI_HELP_SHUTDOWN:
           sig_message(tr("shutdown - Shutdown the server."));
           break; // 54 - HELP_SHUTDOWN
      case DGI_HELP_SHUTOFF:
           sig_message(tr("shutoff - Disable create games, and shutdown the server after all current games are finished."));
           break; // 55 - HELP_SHUTOFF
      case DGI_HELP_TEST:
           sig_message(tr("test - Test a new feature, or bugs. This is a debugging tool. It action may differ, or do nothings."));
           break; // 56 - HELP_TEST
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
  QByteArray out(mesg.toLatin1());

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

#endif // ONLINE_PLAY
