# Wordpress(M.P)

# 1. 서론

---

## 1.1 프로젝트 개요

### 1.1.1 프로젝트 구성 및 목표

1. WEB, DB, DNS 서버를 구축한다. 
2. Wordpress를 만들어 서버 구동을 시켜본다. 
3. 서비스 구축에 의의를 두고 미니 프로젝트 시작
4. 본 구성은 Virtual Box → CentOS를 사용하여 Rocky 리눅스 서버로 구현한다.
5. 도구 : httpd, wordpress, mariaDB, FIrefox, https

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled.png)

| WEB | 192.168.56.100 |
| --- | --- |
| DB | 192.168.56.150 |
| DNS | 192.168.56.200 |
| Gateway | 192.168.56.1 |
- 위 사항들을 적용시켜야 한다. keyword : nmcli

---

# 2. DNS(Domain Name System)

⇒ 특정 컴퓨터의 주소를 찾기 위해, 사람이 이해하기 쉬운 도메인 이름을 숫자로 된 식별 번호(IP주소)로 변환해 준다. (전화번호부에 비유)

- 스마트폰이나 노트북부터 대규모 소매 웹 사이트의 **콘텐츠를 서비스하는 서버에 이르기까지** 인터넷상의 모든 컴퓨터는 숫자를 사용하여 서로를 찾고 통신합니다. 이러한 숫자를 **IP 주소**라고 합니다. 웹 브라우저를 열고 웹 사이트로 이동할 때는 긴 숫자를 기억해 입력할 필요가 없습니다. 그 대신 example.com과 같은 **도메인 이름**을 입력해도 원하는 웹 사이트로 갈 수 있습니다

---

## 2.1 DNS 구성하기

### 2.1.1 네트워크 셋팅  및 설치

1. Rocky-server-gui 파일을 사용하여 구성한다. 
    1. 네트워크 : NAT
        
        ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%201.png)
        
    2. 버전 Red Hat(64bit)
    3. 네트워크 셋팅 
        
        ```bash
        [root@servera ~]
        # nmcli con add con-name **static_dns1** 
        											ifname eth1 
        											type ethernet 
        											ip4 192.168.56.200 
        											gw4 192.168.56.1
        [root@servera ~]
        # nmcli con add con-name static_dns1 ifname eth1 type ethernet ip4 192.168.56.200/24 gw4 192.168.56.1
        
        [root@servera ~]
        # nmcli con up static_dns1
        // connect 오류가 뜰 시 eth1 네트워크 환경을 확인해보라.해당 네트워크가 열려있어야한다.
        [root@servera ~]
        # ip addr show eth1
        ```
        
        DNS의 고정 IP 주소를 할당한다. 192.168.56.200 
        
        Device 셋팅 해준다.
        
        ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%202.png)
        

1. **DNS를 위한 설치**
    
    ```bash
    [root@servera ~]
    # dnf -y install bind
    ```
    

1. **# vim /etc/named.conf 파일 수정 : DNS 서버환경 셋팅**
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%203.png)
    
    - listen-on port 53 : 53번 포트 외부 접근 허용
    
2. systemctl start named
    - named DNS 시스템 실행으로  active된 것을 알 수 있다.
    
    systemctl status named 
    
    ```bash
    # systemctl status named
    ● named.service - Berkeley Internet Name Domain (DNS)
         Loaded: loaded (/usr/lib/systemd/system/named.service; disabled; vendor pr>
         Active: **active** (running) since Thu 2023-11-09 03:06:31 UTC; 7s ago
        Process: 26256 ExecStartPre=/bin/bash -c if [ ! "$DISABLE_ZONE_CHECKING" ==>
        Process: 26259 ExecStart=/usr/sbin/named -u named -c ${NAMEDCONF} $OPTIONS >
       Main PID: 26260 (named)
          Tasks: 6 (limit: 10960)
         Memory: 20.7M
            CPU: 44ms
         CGroup: /system.slice/named.service
                 └─26260 /usr/sbin/named -u named -c /etc/named.conf
    ```
    

## 2.2 DNS 영역

### 2.2.1 정방향

```bash
[root@servera ~]
# cd /var/named

[root@servera named]
# ls
data  dynamic  named.ca  named.empty  named.localhost  named.loopback  slaves

[root@servera named]
# cp named.empty dns.test.example.com.zone

[root@servera named]
# ls
data     named.ca     named.localhost  slaves
dynamic  named.empty  named.loopback   dns.test.example.com.zone

```

1. cd /var/named 아래 있는 named.empty 파일을 복제 해줍니다.
2. vim dns.test.example.com.zone
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%204.png)
    
3. 해당 파일 내용을 수정해줍니다. 
    
    ```bash
    $TTL 3H
    @       IN SOA  dns.test.example.com root.test.example.com. (
                                            0       ; serial
                                            1D      ; refresh
                                            1H      ; retry
                                            1W      ; expire
                                            3H )    ; minimum
            IN NS   dns.test.example.com.  #주 도메인 이름 서버 레코드
            IN A    192.168.56.200         #주 도메인 이름 서버의 IP주소 A레코드
            
    dns     IN A    192.168.56.200  #서브도메인 IP A레코드
    db      IN A    192.168.56.150
    web     IN A    192.168.56.100
    ```
    
    위 앞에 dns db web의 이름이 해당 사이트를 불러 올 때 영향을 주니 주의 해야합니다. 
    
4. 파일 권한 변경하기
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%205.png)
    
    ```bash
    [root@servera named]
    # chown :named dns.test.example.com.zone
    [root@servera named]
    # systemctl restart named
    ```
    
    root에서 named로 바꿔줍니다. 
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%206.png)
    

**영역 정보 설정**

- /etc/named.conf 영역 정보 설정
    - DNS 서버의 연결 정보와 함께 BIND를 통해 서비스할 영역에 대한 설정을 저장합니다.

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%207.png)

해당 영역에 들어가서 영역 설정을 위해 zone을 지정해줍니다.

```bash
zone "dns.test.example.com" IN {
        type master;
        file "dns.test.example.com.zone";
        allow-update { any; };
        allow-trnsfer { any; };
};
```

1. systemctl restart named 해줍니다.

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%208.png)

```bash
[root@servera ~]
# nslookup blog.dns.test.example.com 192.168.56.200
Server:		192.168.56.200
Address:	192.168.56.200#53

Name:	blog.dns.test.example.com
Address: 192.168.56.10
```

1. 정상 호출이 되는지 확인합니다. 

### 2.2.2 역방향 만들기

IP 주소를 검색했을 때 해당 내용으로 이동 시켜 줍니다. 

1. 기존 zone 파일 복사 해서 생성해줍니다.
    
    ```bash
    [root@servera named]
    # cp dns.test.example.com.zone 192.168.56.dns.zone
    
    [root@servera named]
    # vim 192.168.56.chungsik.com
    ```
    
    파일을 생성 후 해당 파일의 역방향 주소를 기입해줍니다.
    
    ```bash
    $TTL 3H
    @       IN SOA  dns.test.example.com root.test.example.com. (
                                            0       ; serial
                                            1D      ; refresh
                                            1H      ; retry
                                            1W      ; expire
                                            3H )    ; minimum
            IN NS   dns.test.example.com.
            IN A    192.168.56.200
    
    101     IN PTR  dns.test.example.com.
    10      IN PTR  db.test.example.com.
    20      IN PTR  wp.test.example.com.
    ```
    
    ESC  : wq 로 저장을 해줍니다.
    
    **전달영역설정** 
    
    /etc/named.conf 파일 수정 
    
    ```bash
    zone "56.168.192.in-addr.arpa" IN {
            type master;
            file "192.168.56.chungsik.com";
            allow-update { any; };
            allow-transfer { any; };
    };
    ```
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%209.png)
    

**권한 변경**

```bash
[root@servera named]
# chown :named 192.168.56.chungsik.com
[root@servera named]
# ls -l
total 24
-rw-r-----. 1 root  named  299 Nov  9 07:33 192.168.56.chungsik.com
drwxrwx---. 2 named named   23 Nov  9 03:06 data
-rw-r-----. 1 root  **named**  277 Nov  9 07:30 dns.test.example.com
drwxrwx---. 2 named named   60 Nov  9 07:18 dynamic
-rw-r-----. 1 root  named 2253 Oct 12 19:23 named.ca
-rw-r-----. 1 root  named  152 Oct 12 19:23 named.empty
-rw-r-----. 1 root  named  152 Oct 12 19:23 named.localhost
-rw-r-----. 1 root  named  168 Oct 12 19:23 named.loopback
drwxrwx---. 2 named named    6 Oct 12 19:23 slaves
```

1. systemctl restart named 
    
    ```bash
    [root@servera named]
    # systemctl restart named
    [root@servera named]
    # systemctl status named
    ● named.service - Berkeley Internet Name Domain (DNS)
         Loaded: loaded (/usr/lib/systemd/system/named.service; disabled; vendor pr>
         Active: active (running) since Thu 2023-11-09 07:43:22 UTC; 4s ago
        Process: 3874 ExecStartPre=/bin/bash -c if [ ! "$DISABLE_ZONE_CHECKING" == >
        Process: 3876 ExecStart=/usr/sbin/named -u named -c ${NAMEDCONF} $OPTIONS (>
       Main PID: 3877 (named)
          Tasks: 6 (limit: 10960)
         Memory: 22.1M
            CPU: 56ms
         CGroup: /system.slice/named.service
                 └─3877 /usr/sbin/named -u named -c /etc/named.conf
    ```
    

/etc/resolv.conf 파일에 nameserver 지정해주기 이것은 컴퓨터를 끄면 해당 파일이 사라지기 때문에 명령어를 써줘야합니다. 

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2010.png)

```bash
# Generated by NetworkManager
nameserver 192.168.56.200
//도메인 주소를 서버에게 물어보는 것 
```

namesever지정을 통해 해당 하는 주소를 거쳐 갈 수 있습니다.

---

DNS 의 정방향 역방향 파일 내부에 db및 web의 주소를 함께 넣어주고 해당 파일을 사용할 수 있도록 셋팅 해 주었습니다.  지금 까지 위 내용을 통해 dns를 구성하고 다음으로 WEB 을 구성해보도록 합시다. 

---

# 3. Web Server

1. 네트워크 셋팅 eth1 에 지정 ip 고정 
    
    ```bash
    [root@servera ~]
    # nmcli con add con-name static_web1 ifname eth1 type ethernet ip4 192.168.56.100/24 gw4 192.168.56.1
    
    [root@servera ~]
    # nmcli con up static_web1
    Connection successfully activated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/4)
    [root@servera ~]
    # nmcli con show
    NAME                UUID                                  TYPE      DEVICE 
    eth0                5fb06bd0-0bb0-7ffb-45f1-d6edd65f3e03  ethernet  eth0   
    static_web1         22861a7e-aac6-47da-b2f9-9fd98a811a32  ethernet  eth1   
    Wired connection 1  930cd0df-234b-3a10-a3ba-f666baa444f9  ethernet  eth2   
    enp0s3              2c12d263-f3e2-440c-8b4e-6ef4e60716ae  ethernet  --     
    System eth1         9c92fad9-6ecb-3e6c-eb4d-8a47c6f50c04  ethernet  --
    ```
    

## 3.1 WEB (httpd , firewalld)

1. httpd 설치 및 방화벽 및 wordpress 설치
    
    ```bash
    #yum -y install httpd
    ```
    

해당 포트가 지정해둔 각각의 포트가 다르니 웹을 지정해준 서버에서는 웹 지정 포트를 사용해서 하니까. 지정해둔 192.168.56.100:80 으로 연다.

1. /etc/httpd/conf/httpd.conf 해당 파일로 들어가서 추가 수정해준다. 
    
    Listen 파일 해당 web의 포트를 지정해 줄 수 있다. 
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2011.png)
    
    ```bash
    [root@servera ~]
    # systemctl start firewalld
    [root@servera ~]
    # systemctl enable httpd --now
    Created symlink /etc/systemd/system/multi-user.target.wants/httpd.service → /usr/lib/systemd/system/httpd.service.
    
    [root@servera ~] //방화벽 열어주기 
    # firewall-cmd --add-service=http --permanent
    success
    
    [root@servera ~]
    # firewall-cmd --reload
    success
    
    [root@servera ~] // 8080포트에 대한 방화벽도 열어준
    # firewall-cmd --add-port=8080/tcp --permanent
    success
    
    [root@servera ~]
    # firewall-cmd --reload
    success
    ```
    
    창에 띄어지기도 한다. 
    
    ```bash
    [root@servera ~]
    # echo apache test >> /var/www/html/index.html
    [root@servera ~]
    # ls /var/www/html
    index.html
    ```
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2012.png)
    

---

# 4. DB

DB는 WAS와 나누는 이유가 있습니다. 

- 프로젝트 관리 정책이나 클라이언트의 요구에 따라 단일 서버를 쓰기도 하지만. 분리하는 이유는
    1. 보안 적인 면에서 서버를 분리할 수록 유리합니다.
    2. DB 서버는 메모리를 빡빡하게 쓰기 떄문에, 리소스를 많이 잡아먹습니다. 그래서 서버분리를 통해 성능을 높일 수 있습니다.
    3. 실무에서는 WAS에서 처리하는게 많은 경우 DB서버와 분리합니다.
- 장점이 있는 반면 단점이 있습니다.
    - 서버 비용의 증가.
    - 방화벽 설정 문제가 종종 발생할 수 있습니다. (관리 포인트가 많을 경우 한 쪽 서버만 접속되는 문제가 발생할 수 있습니다.)
1. 네트워크 셋팅
    - 분리된 DB 서버에서 해당 IP 주소를 변경합니다.
    
    ```bash
    [root@servera ~]
    # nmcli con add con-name static_DB1 ifname eth1 type ethernet ip4 192.168.56.150/24 gw4 192.168.56.1
    
    # nmcli con up static_DB1
    Connection successfully activated (D-Bus active path: /org/freedesktop/NetworkManager/ActiveConnection/3)
    [root@servera ~]
    # ip addr show eth1
    3: eth1: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP group default qlen 1000
        link/ether 08:00:27:00:8c:94 brd ff:ff:ff:ff:ff:ff
        altname enp0s8
        inet **192.168.56.150/24** brd 192.168.56.255 scope global noprefixroute eth1
           valid_lft forever preferred_lft forever
        inet6 fe80::2f99:caf:9084:c43b/64 scope link noprefixroute 
           valid_lft forever preferred_lft forever
    ```
    

## 4.1 DB 구성하기

### 4.1.1 DB 설치 및 시스템 실행하기

- dnf -y install mariadb-server
    
    마리아DB를 사용하기 위해 server용 DB를 설치해 줍니다. 
    
- 마리아 DB의 service를 실행합니다.
    
    ```bash
    [root@servera ~]
    **# systemctl enable mariadb.service --now**
    Created symlink /etc/systemd/system/mysql.service → /usr/lib/systemd/system/mariadb.service.
    Created symlink /etc/systemd/system/mysqld.service → /usr/lib/systemd/system/mariadb.service.
    Created symlink /etc/systemd/system/multi-user.target.wants/mariadb.service → /usr/lib/systemd/system/mariadb.service.
    ```
    

### 4.1.2 maridb 보안 설정

마리아 DB 사용시 기본 보안 설정을 거쳐야 합니다. 

#mysql_secure_installation

- 패스워드를 지정해주고, mysql 사용자 권란을 설정 하며, 원격 및 로컬 장소의 설정 등 데이터베이스 관리등에 관련된 셋팅을 해줍니다.

```bash

```

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2013.png)

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2014.png)

1. 익명 사용자를 제거할 건지 물어봅니다. 만약 ‘y’사용시 mysql -u root 로 로그인 해야 됩니다. ‘n’을 하면 ‘mysql’로도 로그인 되지만. 익명 사용자가 권한을 갖는 것이기 때문에 보안상 지워 주는게 좋습니다. 

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2015.png)

1. 패스워드를 지정해줍니다. 

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2016.png)

1. 누구나 사용자 계정을 생성하지 않고 MariaDB에 로그인 할 수 있는 상태를 의미합니다. 
- 주로 테스트를 위한 용도로 제공되며, 실제 운영 환경에선 보안상 위험할 수 있습니다. 보통은 제거하는 것을 권장합니다.

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2017.png)

1. 보통 root 계정은 ‘localhost’에서만 접속을 허용하는 것이 안전합니다. 원격에서 root계정으로 접속은 보안상 위험할 수 있기에 이 단계에서는 root계정에 대한 원격 로그인을 허용할 것인지 묻고 있습니다. 
    - n을 선택하여 원격 로그인을 비허용 하는 것입니다.
    

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2018.png)

테스트 용 ‘test’데이터 베이스 제거 

1. 기본적으로 MariaDB 설치시 ‘test’라는 이름의 데이터베이스가 생성됩니다. 누구나 이 데이터베이스에 접근할 수 있도록 설정되어있습니다. 이는 주로 테스트를 위하며, 운영 환경에서는 불필요하므로 제거 합니다. 

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2019.png)

권한 테이블 reload

1. 앞서 수행한 변경 사항이 즉시 적용 되도록 MariaDB 권한 테이블을 리로드 하는 것입니다. 

이로 써 MariaDB의 보안 설정이 완료가 됩니다. 

### 4.1.3 MariaDB 사용하기

mysql -u root -p  : root 사용자로 mysql에 접속합니다. 

```bash
[root@servera ~]
# mysql -u root -p 
Enter password: 
Welcome to the MariaDB monitor.  Commands end with ; or \g.
Your MariaDB connection id is 9
Server version: 10.5.22-MariaDB MariaDB Server

Copyright (c) 2000, 2018, Oracle, MariaDB Corporation Ab and others.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

MariaDB [(none)]>
```

데이터 베이스를 확인해 볼 수 있습니다.

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2020.png)

---

# 5. WordPress 구성하기

**위 web+DB+DNS가 구성된 상태에서 진행하게 됩니다**. (이 외로 통합환경에서 wordpress를 구성할 수 있습니다.)

## 5.1 WordPress란?

- 워드프레스는 웹사이트 및 블로그를 만들고 관리하기 위한 오픈 소스 콘텐츠 관리 시스템 이다.
- 사용자들이 웹사이트를 쉽게 관리하고 콘텐츠를 게시할 수 있도록 하는 도구

1. 웹사이트 또는 블로그 사이트 빠르게 시작 및 비용 관리
2. 상시 콘텐츠 관리 간소화 및 IT 운영 능률화
3. 설계 및 기능을 손쉽게 맞춤화 제공

### 5.1.1 Wordpress의 구성 순서

1. SELinux 설정 변경
2. Apache 설치 = WEBserver
3. DataBase(MariaDB)설치 = DBserver
4. PHP 설치 = WEB server
5. Apache & PHP 환경 구성 및 구동 = WEB server
6. MariaDB 환경 구성 및 구동
7. WordPress 다운로드 = web server
8. wordpress용 Apache 가상 호스트 구성 및 적용 = WEBserver

⚠️ 위 내용에서 Apache및 DB 설치가 완료 되었으니 4번 부터 진행합니다. 

---

## 5.2 구성하기

- httpd : webserver
- **php : webserver**
    - dnf -y install php
    - 모듈 설치
        
        ```bash
        [root@servera conf.d]
        # dnf module enable php:8.1
        Last metadata expiration check: 1:41:23 ago on Fri 10 Nov 2023 03:57:38 AM UTC.
        Dependencies resolved.
        ================================================================================
         Package           Architecture     Version             Repository         Size
        ================================================================================
        Enabling module streams:
         php                                8.1                                        
        
        Transaction Summary
        ================================================================================
        
        Is this ok [y/N]: y
        Complete!
        ```
        
        ```bash
        # dnf install php-gd php-soap php-intl php-mysqlnd php-pdo php-pecl-zip php-fpm \php-opcache php-curl php-zip wget
        ```
        

### 5.2.1 패키지 버전 확인

패키지 버전 확인을 합니다.  - Wordpress를 설치하기 전에 해당 도구들이 버전이 맞춰져야 합니다. 그 이하로 사용하게 되면 문제가 발생할 수 있습니다. 

- 확인할 버전 목록
    - httpd : web server
        
        ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2021.png)
        
    - mariaDB : DB server
    - php 버전 확인  : web server
        
        ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2022.png)
        
    - DB 버전확인 : DBserver
        
        ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2023.png)
        

### 5.2.1 apache & PHP 환경 구성 및 구동 하기

- /etc/httpd/conf/httpd.conf에서 환경 셋팅을 해줍니다.
- 해당 파일 167~169 번째 위치한 곳 뒤쪽에 index.php를 추가하여줍니다.
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2024.png)
    
- addtype을 추가해 줍니다.
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2025.png)
    
- /etc/php-fpm.d/www.conf 에 가서 설정을 해줍니다.  wq로 저장
    
    ESC ⇒ /(슬래쉬) ⇒ listen.acl 검색 ⇒ Enter
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2026.png)
    

- systemctl 아파치/php-FPM 프로세스 구동 확인 합니다.
    
    ```bash
    [root@servera html]
    # systemctl enable httpd --now 
    [root@servera html]
    # systemctl enable php-fpm --now
    Created symlink /etc/systemd/system/multi-user.target.wants/php-fpm.service → /usr/lib/systemd/system/php-fpm.service.
    [root@servera html]
    # ps -ef | egrep "httpd|www" //아파치/php-fpm 구동확
    root        4853       1  0 01:40 ?        00:00:03 /usr/sbin/httpd -DFOREGROUND
    apache      4855    4853  0 01:40 ?        00:00:00 /usr/sbin/httpd -DFOREGROUND
    apache      4859    4853  0 01:40 ?        00:00:17 /usr/sbin/httpd -DFOREGROUND
    apache      4860    4853  0 01:40 ?        00:00:17 /usr/sbin/httpd -DFOREGROUND
    apache      4861    4853  0 01:40 ?        00:00:17 /usr/sbin/httpd -DFOREGROUND
    apache      6789    6784  0 06:22 ?        00:00:00 php-fpm: pool www
    apache      6790    6784  0 06:22 ?        00:00:00 php-fpm: pool www
    apache      6791    6784  0 06:22 ?        00:00:00 php-fpm: pool www
    apache      6792    6784  0 06:22 ?        00:00:00 php-fpm: pool www
    apache      6793    6784  0 06:22 ?        00:00:00 php-fpm: pool www
    root        6832    2761  0 06:22 pts/0    00:00:00 grep -E --color=auto httpd|www
    ```
    
- 방화벽 구동 및 설정
    
    ```bash
    [root@servera html]
    # firewall-cmd --permanent --add-service=http //서비스 http 추가
    Warning: ALREADY_ENABLED: http
    success
    [root@servera html]
    # firewall-cmd --reload  // 적용
    success
    [root@servera html]
    # firewall-cmd --list-all  //추가된 서비스 확
    public (active)
      target: default
      icmp-block-inversion: no
      interfaces: eth0 eth1 eth2
      sources: 
      services: cockpit dhcpv6-client http ssh
      ports: 8080/tcp
      protocols: 
      forward: yes
      masquerade: no
      forward-ports: 
      source-ports: 
      icmp-blocks: 
      rich rules:
    ```
    

### 5.2.2 DB 셋팅하기

DBserver로 이동하여 사용자 권한 설정을 해줍니다.

```bash
MariaDB [(none)]> CREATE DATABASE wpdb;
Query OK, 1 row affected (0.010 sec)

MariaDB [(none)]> CREATE USER wpdbadmin@'%' IDENTIFIED BY '1';
Query OK, 0 rows affected (0.015 sec)

MariaDB [(none)]> GRANT ALL ON wpdb.* TO wpdbadmin@'%';
Query OK, 0 rows affected (0.012 sec)

MariaDB [(none)]> FLUSH PRIVILEGES;
Query OK, 0 rows affected (0.001 sec)
```

1. CREATE DATABASE wpdb;
    - wpdb 라는 이름의 데이터베이스를 생성한다.
2. CREATE USER wpdbadmin@’%’ IDENTIFIED BY ‘1’;
    - wpdbadmin이라는 사용자를 생성하여, 비밀번호를 1로 설정하였고, ‘%’는 모든 호스트에서 이 사용자가 접근 가능하도록 허용한다..
3. GRANT ALL ON wpdb.* TO wpdbadmin@’%’;
    - wpdb 데이터베이스에 대한 모든 권한을 wpdbadmin 사용자에게 부여 한다. 이 사용자는 모든 호스트에서 데이터베이스에 대한 모든 작업을 수행할 수 있다.
4. FLUSH PRIVILEGES;
    - 권한 변경사항을 즉시 적용하기 위해 권한을 새로고침 한다. 새로운 사용자 또는 권한을 추가한 후에 항상 FLUSH PRIVILEGES;를 실행해서 변경사항을 적용해야 한다.
    

---

## 5.3 Wordpress

### 5.3.1 wordpress 설치하기

1. wordpress 설치해줍니다.
    
    ```bash
    [root@servera ~]
    # wget https://wordpress.org/latest.tar.gz -O wordpress.tar.gz
    --2023-11-10 07:14:32--  https://wordpress.org/latest.tar.gz
    Resolving wordpress.org (wordpress.org)... 198.143.164.252
    Connecting to wordpress.org (wordpress.org)|198.143.164.252|:443... connected.
    HTTP request sent, awaiting response... 200 OK
    Length: 24479162 (23M) [application/octet-stream]
    Saving to: ‘wordpress.tar.gz’
    
    wordpress.tar.gz    100%[===================>]  23.34M  10.2MB/s    in 2.3s    
    
    2023-11-10 07:14:36 (10.2 MB/s) - ‘wordpress.tar.gz’ saved [24479162/24479162]
    ```
    

> wordpress 파일 압축해제
> 
- tar(tape archive)  명령어로 아카이브 압축해제
    - x : extract  = 아카이브 해제
    - f : file = 아카이브 파일 이름 지정
    
    ```bash
    # tar xf wordpress.tar.gz
    ```
    

> WordPress 파일 권한 설정
> 

```bash
[root@servera ~]
# cp -r wordpress /var/www/html/
[root@servera ~]
# chown -R apache:apache /var/www/html/wordpress/
[root@servera ~]
# chmod -R 775 /var/www/html/wordpress/
[root@servera ~]
# ls -ld /var/www/html/wordpress
drwxrwxr-x. 5 apache apache 4096 Nov 10 07:23 /var/www/html/wordpress
```

### 5.3.2 **가상호스트 준비**

```bash
cp /usr/share/doc/httpd-core/httpd-vhosts.conf /etc/httpd/conf.d/wordpress.conf
```

wordpress 만들어준 복사본에 구성을 셋팅합니다.

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2027.png)

```bash
<VirtualHost *:80>
    ServerAdmin root@test.example.com
    DocumentRoot "/var/www/html/wordpress"
    ServerName wp.test.example.com
    ServerAlias wp
    ErrorLog "/var/log/httpd/dummy-host.example.com-error_log"
    CustomLog "/var/log/httpd/dummy-host.example.com-access_log" common
</VirtualHost>

<Directory "/var/www/html/wordpress">
Option Indexes FollowSymLinks
AllowOverride all
Require all granted
</Directory>
```

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2028.png)

⚠️ 위 내용을 그대로 따라왔다면. 해당 html 파일 및에 구성된 경로를 정확히 알아야한다. php.경로를 못 찾아 페이지 열 때 파일을 찾을 수 없는 오류가 발생할 수 있다. 

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2029.png)

- php  적용을 확인합니다.
    
    ```bash
    vi /var/www/html/phpinfo.php //내부 파일에 들어가 해당 내용을 추가합니다.
    
    추가 할 내용 : <?php phpinfo();?>
    ```
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2030.png)
    

### 5.3.3 Wordpress 구성 설정

- /var/www/html/wordpress/ 디렉토리 내부, wp-config-smaple.php를 복제 하여 wp-config.php를 생성한다.
    
    ```bash
    [root@servera ~]
    # cd /var/www/html/wordpress
    [root@servera wordpress]
    # ls
    index.php        wp-blog-header.php    wp-cron.php        wp-mail.php
    license.txt      wp-comments-post.php  wp-includes        wp-settings.php
    readme.html      wp-config.php         wp-links-opml.php  wp-signup.php
    wp-activate.php  wp-config-sample.php  wp-load.php        wp-trackback.php
    wp-admin         wp-content            wp-login.php       xmlrpc.php
    ```
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2031.png)
    
    ⚠️ DB에서 셋팅해준 DATABASE 명칭과 동일해야 작동이 됩니다. 주의 하세요!
    
    1. 목표 데이터 베이스 설정 
        
        ```bash
        define( 'DB_NAME', 'wpdb' );
        ```
        
    2. 사용할 유저 정보 설정
        
        ```bash
        define( 'DB_USER', 'wpdbadmin' );
        ```
        
    3. 사용할 유저의 비밀 번호 설정
        
        ```bash
        define( 'DB_PASSWORD', '1');
        ```
        
    4. 별도의 DB 서버의 IP 혹은 host 명칭
        
        ```bash
        define( 'DB_HOST', '192.168.56.150' );
        ```
        
    
    ![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2032.png)
    

셋팅이 올바르게 완료 되었다면 

192.168.56.100 을 firefox상에 검색하게 되면 해당 페이지가 떠야 정상적으로 작동했다고 할 수 있습니다. 

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2033.png)

### 5.3.4 Wordpress 로그인

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2034.png)

![Untitled](Wordpress(M%20P)%204bc2dd0df0a846688785a20a3a11c3c7/Untitled%2035.png)

1. DB에서도 정상적으로 기록이 되는 것이 확인이 됩니다. 
    
    ```bash
    MariaDB [wpdb]> use wpdb 
    
    MariaDB [wpdb]> select user_email, display_name from wp_users;
    +--------------------+--------------+
    | user_email         | display_name |
    +--------------------+--------------+
    | chungsik@naver.com | chungsik     |
    +--------------------+--------------+
    1 row in set (0.000 sec)
    ```