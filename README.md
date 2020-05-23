# eserv
* 搬運自Jserv的<以 GDB 重新學習 C 語言程式設計 >教材
* 動機: Jserv的<以 GDB 重新學習 C 語言程式設計 >是非常好的教材;原始代碼放在Google code上,在debian 10上編譯需要修正一些小問題 ; 索性我搬到github來,並狗尾續貂出幾個練習與解答. 

* 原始教材來自於:http://blog.linux.org.tw/~jserv/archives/2010/03/_gdb_c.html ,
並搭配投影片 https://www.slideshare.net/jserv/clang-usinggdb?from_action=save

* 問題1: 在Debian10編譯可能會遇到錯誤,試解之
* 問題2: 根據投影片操作gdb eserv並點擊網頁上面的 <Add>按鈕可能會遇到segmentation fault,試著以gdb解之
  
  
  
  
  
  
  
=====================================================
* 答案1: 打開Makefile,可以看到warning as error(-Werror)打開了,註解後可以正常編譯通過;Warning as error是很好的確保軟體品質的手段之一,建議遇到的時候要一個一個去解,但我們目的是要先進行教材後面的內容,因此先註解掉.
* 答案2: (建議先讀完投影片)
gdb ./eserv進入gdb的交互命令,照著投影片說明,用瀏覽器連上http://127.0.0.1:8000 並點擊1+1<ADD>按鈕 ;程式發生segmentation fault, 
#0  0x00007ffff7e5f206 in ?? () from /lib/x86_64-linux-gnu/libc.so.6
#1  0x00007ffff7e189ef in vfprintf () from /lib/x86_64-linux-gnu/libc.so.6
#2  0x00007ffff7e1f606 in printf () from /lib/x86_64-linux-gnu/libc.so.6
#3  0x000055555555a34a in print_param (pHttp=0x7ffff75c13b0) at libeserv/analysis.c:182
#4  0x000055555555aa23 in cgi_page_sum (pHttp=0x7ffff75c13b0) at cgi_custom.c:10
#5  0x000055555555a7ad in cgi_handler (pHttp=0x7ffff75c13b0, handle=0x55555555a9fe <cgi_page_sum>) at libeserv/cgi.c:23
#6  0x0000555555559a35 in cgiProcess (pHttp=0x7ffff75c13b0) at libeserv/request.c:160
#7  0x0000555555559b30 in replyHandler (pHttp=0x7ffff75c13b0) at libeserv/request.c:192
#8  0x0000555555559d0f in requestHandler (s=0x4) at libeserv/request.c:246
#9  0x00007ffff7f8ffa3 in start_thread () from /lib/x86_64-linux-gnu/libpthread.so.0
#10 0x00007ffff7ec04cf in clone () from /lib/x86_64-linux-gnu/libc.so.6
接著我們將斷點下在適當的點,
(gdb) break cgi_custom.c:10
然後重新執行
(gdb) run 
The program being debugged has been started already.
Start it from the beginning? (y or n) y
程式停在斷點
Starting program: /xxx/eserv/eserv 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[New Thread 0x7ffff7dc3700 (LWP 10303)]
[New Thread 0x7ffff75c2700 (LWP 10310)]

--add.cgi--
[Switching to Thread 0x7ffff75c2700 (LWP 10310)]

Thread 3 "eserv" hit Breakpoint 1, cgi_page_sum (pHttp=0x7ffff75c13b0) at cgi_custom.c:10
我們看一下斷點前後的代碼
(gdb) list
5               const char *lAdd, *rAdd;
6               int sum;
7               char buf[32];
8               printf("\n--add.cgi--\n");
9
10              print_param(pHttp);
11              lAdd = get_param_info(pHttp, "lAdd");
12              rAdd = get_param_info(pHttp, "rAdd");
13              sum = atoi(lAdd) + atoi(rAdd);

現在我懷疑print_param可能導致了seg fault, 我讓gdb跳過這行,程式正常執行完畢
(gdb) jump 11
Continuing at 0x55555555aa23.
[Thread 0x7ffff75c2700 (LWP 10310) exited]
以上只是小小演示了如何用gdb除錯的入門,讀者可以自行延伸,譬如找出seg fault和/或null pointer的root cause. 

