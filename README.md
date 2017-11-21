# gesture-recognition
install cmake, opencv2, run as below insturctions:
```shell
cmake .
make
./main
```

#### how to use
```C++
Gesture gesture(im);
// 存在手返回true, 否则false
gesture.hasHand();
// 返回的vector的大小为空，则没有手势匹配；1，匹配一字；2，匹配八字；
gesture.getFingerLocation(std::vector<cv::Point>)
```
#### step 1: find the hand
<table align="center">
    <tr>
        <td align="center">origin</td>
        <td align="center">py version</td>
        <td align="center">C++ version</td>
    </tr> 
    <tr>
        <td align="center"><img src="pic/1.jpg"></td>
        <td align="center"><img src="pic/4.jpg"></td>
        <td align="center"><img src="pic/3.jpg"></td>
    </tr>
    <tr>
        <td align="center"><img src="pic/2.jpg"></td>
        <td align="center"><img src="pic/5.jpg"></td>
        <td align="center"><img src="pic/6.jpg"></td>
    </tr>
</table>
#### step 2: compared with template gestures by similarity
#### step 3: find the finger point by max convex
max convex:
<div align="center">
    <img src="pic/max_convex.jpg">
</div>

contours of max convex:
<div align="center">
    <img src="pic/max_convex_contours.jpg">
</div>

### step 4: calculate convexity defects

<div align="center">
    <img src="pic/show.png">
</div>

### step 5: keep the contours which depth > threshold, find the finger point
<div align="center">
    <img src="pic/find_finger_point.jpg">
</div>
