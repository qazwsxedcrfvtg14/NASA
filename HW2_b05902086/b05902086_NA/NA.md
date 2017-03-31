#1
    如果兩台機器間的ping過高的話，很容易因為沒有檢測到對面已經發送訊息，所以也同時發送訊息出去造成碰撞。另一個原因則是當使用者增加時，碰撞的機率也會大福的增加。
#2
    (a)
        K=random(0,1,2....(2^m)-1)
        m=min(n,10)
        n=5
        m=5
        K=random(0,1,2....31)
        probability that a node chooses K = 4?
        => 1/32
    (b)
        Carrier Sensing: 96-bit
        Collision Detection: 48-bit
        Exponential Backof: K*512-bit
        4*512/(10*1024*1024) = 1/5120 ~= 2ms
    (c)