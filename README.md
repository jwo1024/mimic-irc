# Mimic-IRC
목차
- [팀원](#팀원)
- [수행한 역할 - jiwolee](#수행한-역할---jiwolee)
- [IRC](#irc)
- [Mimic-irc 사용 및 테스트](#mimic-irc-사용-및-테스트)
- [Mimic-irc 구현사항](#mimic-irc-구현사항)

## 팀원
- jayoon, jchoi, **jiwolee**, jiyun, seoyoo

### 수행한 역할 - jiwolee
  - kqueue-poller 관리
  - client 요청 cmd 파싱
  - IRC RFC 분석 및 프로토콜 cmd 구현
  - bot 기능 추가
  - 수 많은 테스트

## IRC
IRC란 Internet Relay Chat의 약자로 실시간 인터넷 채팅 프로토콜이다. <br/>
서버가 트리구조로 이어져있어 A서버에 붙어있는 클라이언트의 메시지가 B서버에 붙어있는 클라이언트로 relay 이어 달리기 처럼 전해준다.

## Mimic-irc 사용 및 테스트
`nc -C`
`irssi`

## Mimic-irc 구현사항
> IRC RFC 와 irc 서버 프로그램 `inspircd` 을 기준으로 따라 만들었으며, <br/>
> `nc`와 irc 클라이언트 프로그램 `irssi` 를 통해 테스트 하였습니다.  <br/>
> server-to-server가 아닌 단일 서버의 기능만 제공하도록 구현하였습니다.

* I/O Multiplexing
  * `kqueue` 사용
  * `socket`은 Non-block으로 설정
  * 비동기적으로 단일 스레드로 다중 클라이언트를 다룰 수 있도록 함

* 제공하는 프로토콜 Command
  * connection registration <br/> `PASS`, `NICK`, `USER`, `QUIT`
  * channel operator <br/> `JOIN`, `PART`, `TOPIC`, `INVITE`, `KICK`, `PRIVMSG`, `MODE`
  * utils <br/> `PONG`

* 봇
  * 개별 클라이언트로서 작동함
  * 제공하는 프로토콜 Command <br/>  `PRIVMSG`, `QUIT`, `PASS`, `NICK`, `USER`, `JOIN`, `PING`, `PONG`, `PART`
  * 원하는 채팅방에 입퇴장
  * 메시지 출력 기능 <br/>  사용자가 채팅방에 `"Hi, [jiyun | jchoi | jiwolee | seoyoo | jayoon]"`을 입력시 특정 메시지를 응답함
  * 채팅방에 현재시각 출력

## 고민했던 점 & 공부한 것들
- 객체지향적 프로그래밍
  - 팀원들 모두 절차지향언어인 C언어를 베이스로 하여 공부하였기 때문에 Mimic-irc 를 시작하여, <br/> 객체지향적인 구조를 짜는 것에 익숙하지 않았다. <br/>

- I/O Multiplexing 애 대한 이해
  - 어떤 **순서로** 받는지 → 동기, 비동기
  - **기다렸다** 받는지 → 블로킹, 논블로킹
  - 논블로킹 x 비동기

- 갈등사항? 
  - 프로젝트 진행 방식의 합의와 규칙이 없었기에, 작업의 확실한 모듈화를 원하는 팀원들과 전체 프로젝트의 진행을 파악하고 싶어 하는 팀원들 간의 동상이몽이 있었습니다.
  - 또한 전체적인 구조를 제가 이해하고 있었음에도 불구하고, 시간에 쫓긴다는 이유로 충분한 지식 나눔을 하지 못하여 팀원들 간의 지식 격차가 벌어졌습니다.

## Sequence Diagram

초기세팅 & Server build
- img
Client 연결 요청시
- img
Client 메시지 요청시
- img

## 참고자료

* [irc 클라이언트 프로그램](https://irssi.org/support/irc/)
* [irc 서버 프로그램](https://docs.inspircd.org/3/configuration/#server)
* [IRC RFC 1459](https://datatracker.ietf.org/doc/html/rfc1459)
* [Modern irc](https://modern.ircdocs.horse/#user-message)
* [Command example](https://dd.ircdocs.horse/refs/commands/)

