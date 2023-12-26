# Mimic-irc
목차
- [팀원](#팀원)
- [수행한 역할 - jiwolee](#수행한-역할---jiwolee)
- [IRC](#irc)
- [Mimic-irc 사용 및 테스트](#mimic-irc-사용-및-테스트)
- [Mimic-irc 구현사항](#mimic-irc-구현사항)

## 팀원
- jayoon, jchoi, **jiwolee**, jiyun, seoyoo

## 수행한 역할 - jiwolee
  - kqueque-poller 관리
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
IRC RFC 와 irc 서버 프로그램 `inspircd` 을 기준으로 따라 만들었으며, <br/>
`nc`와 irc 클라이언트 프로그램 `irssi` 를 통해 테스트 하였습니다.
+ server-to-server가 아닌 단일 서버의 기능만 제공하도록 구현하였습니다.

* `kqueue`를 활용한 비동기 어쩌구
  * 
* 제공하는 프로토콜 Command
  * 
* 봇
  * 별개의 클라이언트로서 기능
  * 기능

## 고민했던 점 & 공부한 것들
- 객체지향적 프로그래밍
  - 팀원들 모두 절차지향언어인 C언어를 베이스로 하여 공부하였기 때문에 Mimic-irc 를 시작하여, <br/> 객체지향적인 구조를 짜는 것에 익숙하지 않았다. <br/>

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
