
# WizardOfLegend
* What is this project ? 
	* 프로젝트 이름은 WizardOfLegend 이며 스팀플랫폼으로 출시한 인디게임을 모작했습니다.
	* 제작기간은 2020.8.31 ~ 2020.9.7 까지 입니다.
	* 모작한 게임에 대한 정보는 [여기](https://store.steampowered.com/app/445980/Wizard_of_Legend/) 에서 참고하실 수 있습니다.
	* visual studio 2019 , window api , git , paint.net  

[참고 동영상](https://www.youtube.com/watch?v=6xOnUNvwWys)

## 어떻게 구현할 것인가 ?

## 맵
맵은 타일 에디터로 구현 타일 직사각형으로 구현한 이후에 충돌하는 물체 밀어내는 식으로 구현
세로 사이즈 : 124
가로 사이즈 : 146

## 공격
공격 할시에 오브젝트에서 공격방향으로 circle 를 공격시간동안 그린다. circle 에 충돌할 시 공격 판정

공격에 피격 당할시 피격애니메이션을 재생하고 공격 방향 반대방향으로 아주 조금 밀려나며 경직 당한다.

## 쉴드 
쉴드도 circle로 구현하며 쉴드에 공격이 닿으면 공격을 무효화 한다.





