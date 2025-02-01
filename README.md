  STM32 F407VET6을 이용한 디지털 시계 개발
  -----------
  YouTube : https://youtu.be/Ngy91viOW6I
  - 사용 보드 : https://smartstore.naver.com/mhivestore/products/4953744659

  모드 4개(일반 시계, 알람 설정, 스톱워치, 타이머)

- Short Press (0ms ~ 700ms)
- Mid Press (700ms ~ 2500ms)
- Long Press (2500ms ~ )

___

### 모드 1 (일반시계)
  
    버튼 1 -> Mid Press 이상이면 Setmode 진입
  
#### Setmode X

    버튼 2 -> 부저 소리 On/Off
    
    버튼 4 -> 12시간 / 24시간 전환
    
#### Setmode O

    버튼 1

    -> Short Press : 시간 수정 부분 선택 (년, 월, 일, 시, 분, 초)

    -> Mid Press 이상 : Setmode 탈출

    버튼 2

    -> Short Press : 수정 부분 값 1 상승

    -> Mid Press 이상 : 수정 부분 값 연속 상승

    버튼 3

    -> Short Press : 수정 부분 값 1 감소

    -> Mid Press 이상 : 수정 부분 값 연속 감소

___


### 모드 2(알람 설정)
#### 알람 설정 시각과 현재 시계 시각이 동일할 경우 LED와 함께 부저 알람 작동 (버튼 1 Short Press로 알람 멈춤)


    버튼 1 -> Mid press 이상이면 Setmode 진입
    
#### Setmode X
    
    버튼 2 -> 알람 확인 (총 5개의 알람)
    
#### Setmode O

    버튼 1

    -> Short Press : 수정 부분 선택 (알람 인덱스, 알람 On/Off, 시, 분)

    -> Mid Press 이상 : Setmode 탈출

    버튼 2

    -> Short Press : 수정 부분 값 1 상승

    -> Mid Press 이상 : 수정 부분 값 연속 상승

    버튼 3

    -> Short Press : 수정 부분 값 1 감소

    -> Mid Press 이상 : 수정 부분 값 연속 감소

___


### 모드 3(스톱 워치)
#### 스톱워치 기능 + 랩타임 저장 + 랩타임 확인

    버튼 2  -> Short Press : 스톱워치 START / STOP
  
    버튼 3  -> Short Press : 스톱워치 START 시 랩타임 저장(최대 9개) / 스톱워치 STOP 시 랩타임 초기화

    버튼 4  -> Short Press : 스톱워치 랩타임 확인

___



### 모드 4(타이머)
#### 타이머 설정 시각이 끝난 경우 LED와 함께 부저 알람 작동 (버튼 2 Short Press로 알람 멈춤)


    버튼 1 -> Mid press 이상이면 Setmode 진입
  
 #### Setmode X

    버튼 2 -> 타이머 시작
    
#### Setmode O

    버튼 1

    -> Short Press : 수정 부분 선택 (시, 분, 초)

    -> Mid Press 이상 : Setmode 탈출

    버튼 2

    -> Short Press : 수정 부분 값 1 상승

    -> Mid Press 이상 : 수정 부분 값 연속 상승

    버튼 3

    -> Short Press : 수정 부분 값 1 감소

    -> Mid Press 이상 : 수정 부분 값 연속 감소

