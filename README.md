# ConcentrationCardGame
<br>


<img src="https://user-images.githubusercontent.com/92084292/201485152-647d8a6d-e9cb-4604-88f2-c17a76206273.gif">


Card

    앞면 / 뒷면
    종류 ( 3종류 용,늑대,곰 )
    카드 위치 ( x, y )
    클릭하면 뒤집기
    그리기

Game

    컨테이너 ( 8 x 5 = 40 )

    40장 랜덤 배치
        2장씩 40장
    
    카드 선택 (최대 두 장)
    카드 비교 (매칭)
    클리어 조건 판정
    배경 그리기
    카드 전부 그리기
    클릭 수

Additional Feature
    
    2인용 게임
    카드 두 장을 뒤집어서 매칭되면 계속 플레이 -> 점수 +1
    카드 두 장이 매칭되지 않으면 다른 플레이어가 플레이
    모든 카드가 사라졌을때 점수가 높은 플레이어가 승리

Refactoring
    
    GDI+ -> Direct2D, WIC, Actor Class, Bitmap Manager 적용
