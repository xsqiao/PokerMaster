# PokerMaster
基于STC单片机的1V1棋牌游戏
# 作品介绍
《扑克大师》是一款可玩性高、趣味性强的1V1棋牌游戏。游戏引入“送牌”、“抽牌”、“改牌”等全新扑克玩法，并保留了“顺子”、“飞机”、“炸弹”等经典牌型组合，玩法新颖，易于上手，游戏体验极佳。项目综合利用了八位数码管显示、流水灯、三按键、按键消抖、振动传感器、导航按键、红外通信等原理，按键与导航键功能设计符合玩家操作习惯，作品设计精巧，具有较高的应用价值。
本游戏由两名玩家参与，牌面包括1-9共9种。游戏开始时，每位玩家拿到随机20张牌，同一牌面的牌不超过四张。系统随机决定先手玩家。数码管与LED通过闪烁的方式实时显示当前牌面与页数，并标识当前有无出牌权。玩家按操作手册进行操作，先打出手中所有牌的玩家获胜，游戏结束，玩家将看到胜利/失败表情，并被告知当前所有牌的牌面之和。
# 游戏规则说明

1. 本游戏由两名玩家参与。
1. 牌面包括：1，2，3，4，5，6，7，8，9共9种。
1. 游戏开始时，每位玩家拿到随机20张牌，同一牌面的牌不超过四张。
1. 系统随机决定先手。玩家每次可以选择执行a~f中的任一操作，之后由对方执行操作。
1. 先打出手中所有牌的玩家获胜。游戏结束后，玩家将看到胜利/失败表情，并被告知当前所有牌的牌面之和。
1. 每一页显示8张牌，LED灯（L7~L0）代表页数及当前所在页面。
1. 特殊牌型说明：
a)	顺子：5张以上连续牌组成顺子。
b)	飞机：拥有连续2类牌，且每种牌3张。
c)	炸弹：拥有某一类牌4张。
1. 操作说明：
a)	使用顺子：将光标移动至顺子中牌面最小的牌所在位置，按下K3键，系统自动选择使用最长的顺子。顺子消失，对手得到顺子中随机一张牌。
b)	使用飞机：将光标移动至飞机位置，按下K2使用飞机。飞机起飞并消失，玩家可以指定自己的2张牌给对手。
c)	使用炸弹：将光标移动至炸弹位置，按下K1使用炸弹。炸弹引爆，玩家可以指定自己的2张牌消失。
d)	送牌：拨动“上”导航键，对手得到当前所选的牌。
e)	取牌：拨动“下”导航键，玩家得到牌库中的任一一张牌。
f)	改牌：按下导航键，当前所选的牌面增加1。若当前牌面为9，将更改为1。
g)	洗牌：振动学习板，系统将牌面按升序排序。需在执行a~f之前进行。
h)	（被动）收牌：当对手使用顺子、飞机或送牌时，玩家将得到一张牌，并放在起始位置。



# 作品展示
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2636934/1661432646291-21ec2502-2adc-47df-8af2-d85c34185f00.png#clientId=u3b889613-a486-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=765&id=ueab5c08a&margin=%5Bobject%20Object%5D&name=image.png&originHeight=765&originWidth=529&originalType=binary&ratio=1&rotation=0&showTitle=false&size=855379&status=done&style=none&taskId=ue7ad0065-bc66-4a94-8cdf-44cb37b8464&title=&width=529)
图2.1 游戏欢迎界面 
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2636934/1661432659121-d6a68f2e-376c-48d6-bedc-eb3d34cc56f9.png#clientId=u3b889613-a486-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=762&id=u9d3c4186&margin=%5Bobject%20Object%5D&name=image.png&originHeight=762&originWidth=551&originalType=binary&ratio=1&rotation=0&showTitle=false&size=847990&status=done&style=none&taskId=u350cfc65-dcd1-48b2-82a4-0a705711f0f&title=&width=551)
图2.2 游戏进行界面，此时下方玩家拥有发牌权
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2636934/1661432669166-3871cda0-b0b6-413b-8dba-2feed0cf7118.png#clientId=u3b889613-a486-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=722&id=u61f87fd2&margin=%5Bobject%20Object%5D&name=image.png&originHeight=722&originWidth=533&originalType=binary&ratio=1&rotation=0&showTitle=false&size=800168&status=done&style=none&taskId=u823d631a-bab4-437f-9897-e415b5edfd1&title=&width=533)
图2.3 游戏结束界面，下方玩家获胜
![image.png](https://cdn.nlark.com/yuque/0/2022/png/2636934/1661432682618-470dd498-d1db-4e3f-9a78-0e12849431bf.png#clientId=u3b889613-a486-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=713&id=udde4cb7f&margin=%5Bobject%20Object%5D&name=image.png&originHeight=713&originWidth=502&originalType=binary&ratio=1&rotation=0&showTitle=false&size=692990&status=done&style=none&taskId=u74777e7f-0d81-477c-a84b-30304d678f5&title=&width=502)
图2.4 振动洗牌后的闪烁显示状态
# 设计思路
项目设计主要分为两个大板块：含随机发牌、振动洗牌、“送牌”、“取牌”、“改牌”相应代码实现的单机布局以及基于485双机通信原理的1V1布局。项目设计核心主要包括：使用怎样的数据结构存储手中的牌、如何使用高效算法实现检测并使用特殊牌的功能、如何实现数码管与LED某一特定位的闪烁从而标识当前选中的牌面与页码、1V1如何进行初始化以确定初始发牌权、游戏过程中如何保证发牌权互斥并实时提醒玩家发牌权的有无。
# 操作手册

1. 以下操作可在游戏全程执行：
a)	振动学习板：启动“摇一摇理牌”，系统将牌面按升序排序。
b)	拨动导航键左键：切牌，选中上一张牌。若当前选中第一张牌，该操作将选择最后一张牌。
c)	拨动导航键右键：切牌，选中下一张牌。若当前选中最后一张牌，该操作将选择第一张牌。
1. 以下操作当且仅当拥有出牌权时可以执行，执行完毕之后出牌权将自动移交给对手：
a)	按下导航键：改牌，当前所选中的牌面加1。若当前牌面为9，该操作将牌面更改为1。
b)	拨动导航键上键：送牌，玩家打出选中的牌，对手得到该牌。
c)	拨动导航键下键：抽牌，玩家从牌库中抽取任意一张牌。
d)	按下K3：使用顺子，系统将选择从当前选中牌开始的、最长的顺子（5张以上牌面连续的牌组成顺子）。玩家打出顺子，对手得到顺子中随机一张牌。
e)	按下K2：使用飞机，系统将选择从当前选中牌开始的、最大的飞机（2种以上牌面连续、每种牌需3张的牌，组成飞机）。玩家打出飞机，并得到两次送牌机会，切牌之后再次按下K2确定送牌。
f)	按下K1：使用炸弹，系统将选择当前选中牌所对应的、最大的炸弹（4张以上牌面相同的牌组成炸弹）。玩家打出炸弹，并得到两次消牌机会，切牌之后再次按下K1确定消牌。
1. 以下操作被动执行：
当对手使用顺子、飞机或执行送牌操作时，玩家将得到对手指定的牌，系统将该牌放在起始位置。
