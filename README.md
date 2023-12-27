# irc readme.md

# Mimic-IRC

{우리는 IRC 서버를 만들었다.}
![작동스크린샷-WellcomeMsg-white](readme-img/작동스크린샷-WellcomeMsg-white.png)
![작동스크린샷-채팅방과DM](readme-img/작동스크린샷-채팅방과DM.png)
![작동스크린샷-채팅방과BOT](readme-img/작동스크린샷-채팅방과BOT.png)

---

# 목차

- [팀원](https://github.com/jwo1024/Mimic-IRC#%ED%8C%80%EC%9B%90)
- [수행한 역할 - jiwolee](https://github.com/jwo1024/Mimic-IRC#%EC%88%98%ED%96%89%ED%95%9C-%EC%97%AD%ED%95%A0---jiwolee)
- [IRC](https://github.com/jwo1024/Mimic-IRC#irc)
- [Mimic-irc 사용 및 테스트](https://github.com/jwo1024/Mimic-IRC#mimic-irc-%EC%82%AC%EC%9A%A9-%EB%B0%8F-%ED%85%8C%EC%8A%A4%ED%8A%B8)
- [Mimic-irc 구현사항](https://github.com/jwo1024/Mimic-IRC#mimic-irc-%EA%B5%AC%ED%98%84%EC%82%AC%ED%95%AD)

## 팀원

- jayoon, jchoi, **jiwolee**, jiyun, seoyoo

### 수행한 역할 - jiwolee

- kqueue-poller 관리
- client 요청 cmd 파싱
- IRC RFC 분석 및 프로토콜 cmd 구현
- bot 기능 추가
- 수 많은 테스트..

# 목표

- Non-relay의 소규모 서버 : 다중 서버간 릴레이를 배제한, 단일 서버 기능만 제공하도록 구현
- I/O multiplexing : 단일 스레드/프로세스 상에서 다중 클라이언트에 대한 poller기반의 비동기처리를 지원
    - BSD 기반 시스템(우리의 경우 macOS)에서 활용 가능한, 보다 고성능인 kqueue를 poller로 사용
- IRC Protocol의 주요 명령어 기능 제공
    - connection registration : `PASS`, `NICK`, `USER`, `QUIT`
    - channel operator : `JOIN`, `PART`, `TOPIC`, `INVITE`, `KICK`, `PRIVMSG`, `MODE`
    - utils : `PONG`
- 서버 상에서 클라이언트와 동등하게 인식되는 chatting bot 프로그램 개발

## IRC 란?

IRC란 Internet Relay Chat의 약자로 실시간 인터넷 채팅 프로토콜이다.

서버가 트리구조로 이어져있어 A서버에 붙어있는 클라이언트의 메시지가 B서버에 붙어있는 클라이언트로 relay 이어 달리기 처럼 전해준다.

## 접근법

1. [RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459) - IRC 프로토콜에 관한 공식 문서를 읽으면서, 서버가 만족해야할 조건들을 개괄적으로 분석했다.
2. 권한 및 패키지 관련 이점을 위해 도커 위에 우분투 컨테이너를 올리고 테스트했다.
    1. `tcpflow`로 서버와 클라이언트가 tcp 패킷 상에서 실제로 주고받는 메시지를 모니터링했다. 
    2. `insplRCd` - 오픈 소스 IRC 서버를 실행하여 실제 반응하는 방식을 분석하였다.
    3. `irrsi` - 텍스트 기반 IRC 클라이언트 프로그램
    4. `nc` (netcat) - 기본적인 네트워킹 도구로, 보다 raw하게 클라이언트 역할을 테스트 할 수 있었다.
3. 1과 2번의 과정을 통해 우리가 만들고자 하는 서버가 제공해야하는 기능을 구체화할 수 있었음.

## Mimic-irc 구현사항

- C++
- I/O Multiplexing
    - `kqueue` 사용
    - `socket`은 Non-block으로 설정
- 제공하는 프로토콜 Command
    - connection registration`PASS`, `NICK`, `USER`, `QUIT`
    - channel operator `JOIN`, `PART`, `TOPIC`, `INVITE`, `KICK`, `PRIVMSG`, `MODE`
    - utils`PONG`
- 봇
    - 개별 클라이언트로서 작동함
    - 제공하는 프로토콜 Command`PRIVMSG`, `QUIT`, `PASS`, `NICK`, `USER`, `JOIN`, `PING`, `PONG`, `PART`
    - 원하는 채팅방에 입퇴장
    - 메시지 출력 기능사용자가 채팅방에 `"Hi, [jiyun | jchoi | jiwolee | seoyoo | jayoon]"`을 입력시 특정 메시지를 응답함
    - 채팅방에 현재시각 출력

## 고민했던 점 & 공부한 것들

- 객체지향적 프로그래밍
    - 팀원들 모두 절차지향언어인 C언어를 베이스로 하여 공부하였기 때문에 Mimic-irc 를 시작하여,객체지향적인 구조를 짜는 것에 익숙하지 않았다.
    - 객체 지향적으로 만드려고 노력했다 어필하고 싶음
- I/O Multiplexing 애 대한 이해
    - 어떤 **순서로** 받는지 → 동기, 비동기
    - **기다렸다** 받는지 → 블로킹, 논블로킹
    - 논블로킹 x 비동기
- 갈등사항?
    - 프로젝트 진행 방식의 합의와 규칙이 없었기에, 작업의 확실한 모듈화를 원하는 팀원들과 전체 프로젝트의 진행을 파악하고 싶어 하는 팀원들 간의 동상이몽이 있었습니다.
    - 또한 전체적인 구조를 제가 이해하고 있었음에도 불구하고, 시간에 쫓긴다는 이유로 충분한 지식 나눔을 하지 못하여 팀원들 간의 지식 격차가 벌어졌습니다.

## Mimic-irc 사용 및 테스트

Mimic-irc
- 기본 포트번호 `6667`
- 기본 패스워드 `password`

Mimic-irc 서버 실행하기
-  `make run`
- 포트번호나 패스워드 변경하고 싶다면 아래와 같이 실행 <br/> `./bin/ircserv <port> <password>`

BOT
- 기본 포트번호 `6667`
- 기본 패스워드 `password`

Bot 실행 (T800)
- `cd BOT`
- `make run`

- 포트번호 변경하기 <br/>
  `defines.hpp` 의 `kDefaultPort`,`kDefaultServerPort` 를 원하는 포트번호로 변경 후 `make run`
- 패스워드 변경하기 <br/>
`defines.hpp` 의 `kDefaultPassword`,`kDefaultServerPassword` 를 원하는 패스워드로 변경

클라이언트 프로그램 실행하기
- `irssi -c [ip주소] -p [포트번호] -n [닉네임]`
- `irssi -c 127.0.0.1 -p 6667 -n nickname`

- 클라이언트 프로그램안에서 채팅방 만들거나 참여하기 <br/> `/join [채팅방이름]`
- 클라이언트 프로그램안에서 dm 보내기 <br/> `/msg [상대닉네임] [메시지내용]`
- 클라이언트 프로그램안에서 채팅방 주제 설정하기 <br/>  `/topic [주제]`
- 기타 등등.. 

참고한 IRC 실제 서버
- `inspircd --runasroot --nofork`
tcpflow
- `tcpflow -i lo port 6667 -c`
- `sudo tcpflow -i [네트워크 인터페이스] port [포트번호] -c`
- `ifconfig | grep LOOPBACK` 의 결과로 나오는 네트워크 인터페이스 감청(?)
- tcpflow 사용시 docker 사용했던 이유 → sudo 권한 필요해서

nc
- `nc -C 127.0.0.1 8080`
- `nc -C [ip주소] [포트번호]`

## Sequence Diagram

- 초기세팅 & Server build
  ![다이어그램-초기세팅_Serverbuild](readme-img/다이어그램-초기세팅_Serverbuild.png)
- Client 연결 요청시
  ![다이어그램-Client_요청시](readme-img/다이어그램-Client_요청시.png)
- [다이어그램 전체 구조설계](readme-img/다이어그램-전체_구조설계.png)

## 참고자료

- [irc 클라이언트 프로그램](https://irssi.org/support/irc/)
- [irc 서버 프로그램](https://docs.inspircd.org/3/configuration/#server)
- [IRC RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459)
- [Modern irc](https://modern.ircdocs.horse/#user-message)
- [Command example](https://dd.ircdocs.horse/refs/commands/)
