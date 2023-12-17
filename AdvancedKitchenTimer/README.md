# AdvancedKitchenTimer
自由課題のキッチンタイマー。
砲丸型ランプを利用した複数タイマーの切り替えと、可変抵抗による時間設定の加減算速度の変更を実装した。

## 依存ライブラリ
- TimerOne
    - 割り込みに利用する
- pitches.h
    - ビープ音に利用する
- Queue
    - 他のタイマーがビープ音を鳴らしている最中にタイマーが終了したとき、ビープ音を待つために利用する
    - https://github.com/sdesalas/Arduino-Queue.h
- StackArray-master
    - 他のLEDを操作中にタイマーが鳴った時に元のLED位置を記憶するために利用する
    - https://github.com/oogre/StackArray
