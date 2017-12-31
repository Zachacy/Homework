姓名: 王淮慕
學號: 606415050
E-mail:tpre31738@gmail.com
作業環境: Ubuntu 64bit
編譯器: Qt

指定作業功能:
1.請使用FLTK和OpenGL撰寫程式

2.使用者介面 : (10%)
包含繪圖畫布、按鈕…等。按鈕無功能則不予計分。

3.Draw mode(left view) : (55%)
1) 繪圖按鈕包含points、lines、triangles、quadrilaterals、circles
2) 圖形顏色依序為白、紅、綠、藍、黃
3) 每個圖形於畫布上可顯示數量應為無限
4) 畫布局部放大功能(註)

4.Draw bmp image(right view)：(20%)
需要有一個read image按鈕來將bmp圖檔繪製於視窗上。
(※採一個個pixel寫入的方式，而非直接顯示圖檔)

5.Other : (15%)
1) 程式視窗必須能夠縮放，並不破壞已繪出之圖形
2) Clear button - 清除畫布上所有形狀(僅清除draw mode之繪圖畫布)
3) idle button - 將狀態還原為程式剛開啟之狀態
4) exit button - 結束程式

額外bonus操作功能:
當視窗(windows)縮放時,畫布中的圖也會跟著縮放，但Disney.bmp不會跟著縮放
(自由發揮，依照全班整體作業評量，ex:增加圖形種類、依使用者喜好更換顏色、錄製Demo影片(2%)等。)

Linux 環境 : 
$ cd ~/FLTK/build
$ cmake ..;make
$ cd ~/FLTK/bin
$ ./FLTK

Windows 環境 : 
1.先架設好vusual studio 專案 (含 FLTK , openGL相關環境變數)
2. 在資料夾 FLTK/src 下有主程式檔(main.cpp)。複製visual studio專案下
3.將 main.cpp建置主程式檔(source.cpp) build-> run
(資料夾 "FLTK_windows" 裡有windows專案檔)
實際模擬 : Demo.mkv
