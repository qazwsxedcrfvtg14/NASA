# Network Administration 1
## 1.
    登入ip為140.112.31.252的這台機器(這台應該是網管的管理範圍)，在上面使用show arp或是arp -a後找到140.112.31.254這ip所對應到的mac address(使用 | include 140.112.31.254 )，之後到switch上打show mac-address-table然後找到出事的電腦所連接到的port(使用 | include 那台電腦的mac address )，然後過程中如果可能會經過多個switch，因此要利用show mac-address-table沿線慢慢找到出事的電腦所連接的port。
## 3.
    (a)PC2可以ping到PC3，如果不行，可以檢查看看兩台電腦的網段是否相同
    (b)如果兩組vlan是屬於不同網段的話可以使用gateway(router)來連接兩個不同的vlan。如果是屬於相同網段但沒有撞ip的話，則可以把vlan換成trunk模式接起來。
    (c)MayIeatEat?