# NASA 2017 Final Project - NFS 伺服器靜態負載平衡
## Github Location
* https://github.com/qazwsxedcrfvtg14/NASA/tree/master/Final/
## 組員
* 資工一 b05902086 周逸  
* 資工二 b04902112 張凱捷  
* 資工一 b05902092 謝耀慶  
* 資工一 b05902093 顏睿楠  
## 題目敘述
## 系統簡介
* 架構  
    我們這個系統的架構是由一台 DNS Server、一台 NFS Master Server、多台 NFS Server和多台 Client 所組成。
    * DNS Server  
        一台控制域名用的機器，這台機器可以讓發生緊急意外的時能將其他機器所對應到的域名對應到其他地方。  
        並且能讓管理系統的人不需要去直接把每一台機器的IP直接記起來。  
    * NFS Master Server
        一台存放著各台系統和NFS負載相關設定檔的位置，但是當這台機器故障時就需要透過DNS將指向這台機器的域名換到其他機器上。  
        而管理員要操作各項關於NFS負載平衡的設定時，也只要在這台機器上操作即可。  
        而且這台機器也能身兼NFS Server。
    * NFS Server
        存放著使用者資料的機器，會在開機時自動掛載NFS Master Server上的設定檔目錄，藉此來達成中央控管功能。
    * Client
        會在開機時自動掛載NFS Master Server上的設定檔目錄，透過這樣當方式得到autofs的設定目錄，並且取得NFS備援機器的位置。
        也能藉由這樣的方式在當管理員變更NFS負載平衡設定時就能自動地把使用者的目錄重新掛載(但不會中斷使用者的連線)。
* Hash function  
    這個是一個給管理員使用的程式，當管理員可以透過執行他並加上參數達成調整NFS靜態負載目的。  
## 實作過程
* Hash function
    * A balanced lookup table
    * Auto balance itself when groups or machines are added/removed
    * balanced backup (load are still balanced when some machines are down)
    * Minimize the transfer needed
    * Minimize the groups affected when two (or more) machines are down (a direct benefit from balanced backup)
    * Theoretically can have infinite numbers of groups or machines (no fixed-sized data structure used in the implementation)
* Moving files  
    當要把一個group從B機器搬到C機器時，相較於把B機器跟C機器的檔案直接mount在一台中央控制A機器上後再搬移檔案，我們使用直接移動的方式，也直接在B機器上把C機器上檔案mount上去後，再把資料搬過去。這樣可以讓資料不會需要全部都通過A機器後再到達目的地，也能減少約一半的網路開銷。  
    ![img](1.png)
    ![img](2.png)  
    我們使用了兩種不同的方式來時做這個過程來移動檔案  
    * By NFS bootstrap
        這裡我們在每台機器上執行了一個自己寫的Service，而那服務的內容是每過一段時間就去檢查NFS master
        * 那個
    * By SSH
        * move group0 from nfs1 to nfs2
        ```
        ssh root@nfs1 “./movefile nfs2 group0”
        ```
        * add group0 to nfs1
        ```
        ssh root@nfs1 “mkdir /var/nfs/group0”
        ```
        * remove group0 from nfs1
        ```
        ssh root@nfs1 “rm -rf /var/nfs/group0”
        ```
        * movefile.sh
        ```
        ip=$1
        dir=$2
        mount -t nfs $ip:/var/nfs /mnt/$ip
        cp -rf /var/nfs/$dir /mnt/$ip
        umount /mnt/$ip
        ```
* DNS
    * NFS server 由 autofs 經過 DNS server 連結，於是可以做到多一層備援
## 遇到的困難
## 未來的可能性
* 把各組別的人數和優先權納入平衡附載的考量
* 目前搬移使用者會需要將所有使用者登出(因為要重啟autofs),但可以做到只把要搬移的使用者登出就好
* 最好是能做到完全沒有down time的搬移(等使用者自己登出，下次登入就會在新的機器上)，但是autofs不支援
## 分工
 
 
 
 
