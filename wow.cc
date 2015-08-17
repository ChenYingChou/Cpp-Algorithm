/* wow.cc -- tonychen@finenet.com.tw

   魔兽世界(World of Warcraft)终极版

   描述
    魔兽世界的西面是红魔军的司令部，东面是蓝魔军的司令部。两个司令部之间是依次
    排列的若干城市，城市从西向东依次编号为1,2,3 .... N ( N <= 20 )。红魔军的司
    令部算作编号为0的城市，蓝魔军的司令部算作编号为N+1的城市。司令部有生命元，
    用于制造武士。

    两军的司令部都会制造武士。武士一共有 dragon 、ninja、iceman、lion、wolf 五
    种。每种武士都有编号、生命值、攻击力这三种属性。

    双方的武士编号都是从1开始计算。红方制造出来的第 n 个武士，编号就是n。同样，
    蓝方制造出来的第 n 个武士，编号也是n。

    武士在刚降生的时候有一个初始的生命值，生命值在战斗中会发生变化，如果生命值
    减少到0（生命值变为负数时应当做变为0处理），则武士死亡（消失）。

    有的武士可以拥有武器。武器有三种，sword, bomb,和arrow，编号分别为0,1,2。

    武士降生后就朝对方司令部走，在经过的城市如果遇到敌人（同一时刻每个城市最多
    只可能有1个蓝武士和一个红武士），就会发生战斗。每次战斗只有一方发起主动进攻
    一次。被攻击者生命值会减去进攻者的攻击力值和进攻者手中sword的攻击力值。被进
    攻者若没死，就会发起反击，被反击者的生命值要减去反击者攻击力值的一半(去尾取
    整)和反击者手中sword的攻击力值。反击可能致敌人于死地。

    如果武士在战斗中杀死敌人 (不论是主动进攻杀死还是反击杀死)，则其司令部会立即
    向其发送8个生命元作为奖励，使其生命值增加8。当然前提是司令部得有8个生命元。
    如果司令部的生命元不足以奖励所有的武士，则优先奖励距离敌方司令部近的武士。

    如果某武士在某城市的战斗中杀死了敌人，则该武士的司令部立即取得该城市中所有
    的生命元。注意，司令部总是先完成全部奖励工作，然后才开始从各个打了胜仗的城
    市回收生命元。对于因司令部生命元不足而领不到奖励的武士，司令部也不会在取得
    战利品生命元后为其补发奖励。

    如果一次战斗的结果是双方都幸存 (平局)，则双方都不能拿走发生战斗的城市的生
    命元。

    城市可以插旗子，一开始所有城市都没有旗子。在插红旗的城市，以及编号为奇数的
    无旗城市，由红武士主动发起进攻。在插蓝旗的城市，以及编号为偶数的无旗城市，
    由蓝武士主动发起进攻。

    当某个城市有连续两场战斗都是同一方的武士杀死敌人(两场战斗之间如果有若干个战
    斗时刻并没有发生战斗，则这两场战斗仍然算是连续的；但如果中间有平局的战斗，
    就不算连续了) ，那么该城市就会插上胜方的旗帜，若原来插着败方的旗帜，则败方
    旗帜落下。旗帜一旦插上，就一直插着，直到被敌人更换。一个城市最多只能插一面
    旗帜，旗帜没被敌人更换前，也不会再次插同颜色的旗。

    各种武器有其特点：

    sword武器的初始攻击力为拥有它的武士的攻击力的20%（去尾取整）。但是sword每经
    过一次战斗(不论是主动攻击还是反击)，就会变钝，攻击力变为本次战斗前的80% (去
    尾取整)。sword攻击力变为0时，视为武士失去了sword。如果武士降生时得到了一个
    初始攻击力为0的sword，则视为武士没有sword.

    arrow有一个攻击力值R。如果下一步要走到的城市有敌人，那么拥有arrow的武士就会
    放箭攻击下一个城市的敌人（不能攻击对方司令部里的敌人）而不被还击。arrow使敌
    人的生命值减少R，若减至小于等于0，则敌人被杀死。arrow使用3次后即被耗尽，武
    士失去arrow。两个相邻的武士可能同时放箭把对方射死。

    拥有bomb的武士，在战斗开始前如果判断自己将被杀死 (不论主动攻击敌人，或者被
    敌人主动攻击都可能导致自己被杀死，而且假设武士可以知道敌人的攻击力和生命值)，
    那么就会使用bomb和敌人同归于尽。武士不预测对方是否会使用bomb。

    武士使用bomb和敌人同归于尽的情况下，不算是一场战斗，双方都不能拿走城市的生
    命元，也不影响城市的旗帜。

    不同的武士有不同的特点。

    dragon可以拥有一件武器。编号为n的dragon降生时即获得编号为 n%3 的武器。dragon
    还有“士气”这个属性，是个浮点数，其值为它降生后其司令部剩余生命元的数量除以
    造dragon所需的生命元数量。dragon 在一次在它主动进攻的战斗结束后，如果还没有
    战死，而且士气值大于0.8，就会欢呼。dragon每取得一次战斗的胜利(敌人被杀死)，
    士气就会增加0.2，每经历一次未能获胜的战斗，士气值就会减少0.2。士气增减发生在
    欢呼之前。

    ninjia可以拥有两件武器。编号为n的ninjia降生时即获得编号为 n%3 和 (n+1)%3的
    武器。ninja 挨打了也从不反击敌人。

    iceman有一件武器。编号为n的iceman降生时即获得编号为 n%3 的武器。iceman 每前
    进两步，在第2步完成的时候，生命值会减少9，攻击力会增加20。但是若生命值减9后
    会小于等于0，则生命值不减9,而是变为1。即iceman不会因走多了而死。

    lion 有“忠诚度”这个属性，其初始值等于它降生之后其司令部剩余生命元的数目。
    每经过一场未能杀死敌人的战斗，忠诚度就降低K。忠诚度降至0或0以下，则该lion逃
    离战场,永远消失。但是已经到达敌人司令部的lion不会逃跑。Lion在己方司令部可能
    逃跑。lion 若是战死，则其战斗前的生命值就会转移到对手身上。所谓“战斗前”，
    就是每个小时的40分前的一瞬间。

    wolf降生时没有武器，但是在战斗中如果获胜 (杀死敌人)，就会缴获敌人的武器，但
    自己已有的武器就不缴获了。被缴获的武器当然不能算新的，已经被用到什么样了，
    就是什么样的。

    以下是不同时间会发生的不同事件：

    在每个整点，即每个小时的第0分， 双方的司令部中各有一个武士降生。

    红方司令部按照 iceman、lion、wolf、ninja、dragon 的顺序制造武士。

    蓝方司令部按照 lion、dragon、ninja、iceman、wolf 的顺序制造武士。

    制造武士需要生命元。

    制造一个初始生命值为 m 的武士，司令部中的生命元就要减少 m 个。

    如果司令部中的生命元不足以制造某武士，那么司令部就等待，直到获得足够生命元
    后的第一个整点，才制造该武士。例如，在2:00，红方司令部本该制造一个 wolf ，
    如果此时生命元不足，那么就会等待，直到生命元足够后的下一个整点，才制造一个
    wolf。

    在每个小时的第5分，该逃跑的lion就在这一时刻逃跑了。

    在每个小时的第10分：所有的武士朝敌人司令部方向前进一步。即从己方司令部走到
    相邻城市，或从一个城市走到下一个城市。或从和敌军司令部相邻的城市到达敌军司
    令部。

    在每个小时的第20分：每个城市产出10个生命元。生命元留在城市，直到被武士取走。

    在每个小时的第30分：如果某个城市中只有一个武士，那么该武士取走该城市中的所
    有生命元，并立即将这些生命元传送到其所属的司令部。

    在每个小时的第35分，拥有arrow的武士放箭，对敌人造成伤害。放箭事件应算发生
    在箭发出的城市。注意，放箭不算是战斗，因此放箭的武士不会得到任何好处。武士
    在没有敌人的城市被箭射死也不影响其所在城市的旗帜更换情况。

    在每个小时的第38分，拥有bomb的武士评估是否应该使用bomb。如果是，就用bomb和
    敌人同归于尽。

    在每个小时的第40分：在有两个武士的城市，会发生战斗。 如果敌人在5分钟前已经
    被飞来的arrow射死，那么仍然视为发生了一场战斗，而且存活者视为获得了战斗的
    胜利。此情况下不会有 "武士主动攻击"、"武士反击"、"武士战死" 的事件发生，但
    战斗胜利后应该发生的事情都会发生。如Wolf一样能缴获武器，旗帜也可能更换，等
    等。在此情况下,Dragon同样会通过判断是否应该轮到自己主动攻击来决定是否欢呼。

    在每个小时的第50分，司令部报告它拥有的生命元数量。

    在每个小时的第55分，每个武士报告其拥有的武器情况。

    武士到达对方司令部后就算完成任务了，从此就呆在那里无所事事。

    任何一方的司令部里若是出现了2个敌人，则认为该司令部已被敌人占领。

    任何一方的司令部被敌人占领，则战争结束。战争结束之后就不会发生任何事情了。

    给定一个时间，要求你将从0点0分开始到此时间为止的所有事件按顺序输出。事件及
    其对应的输出样例如下：

    1) 武士降生
     输出样例：
     000:00 blue lion 1 born
     表示在 0点0分，编号为1的蓝魔lion武士降生
     如果造出的是dragon，那么还要多输出一行，例：

     000:00 blue dragon 1 born
     Its morale is 23.34
     表示该该dragon降生时士气是23. 34(四舍五入到小数点后两位)

     如果造出的是lion，那么还要多输出一行，例:
     000:00 blue lion 1 born
     Its loyalty is 24

     表示该lion降生时的忠诚度是24

    2) lion逃跑
     输出样例：
     000:05 blue lion 1 ran away
     表示在 0点5分，编号为1的蓝魔lion武士逃走

    3) 武士前进到某一城市
     输出样例：
     000:10 red iceman 1 marched to city 1 with 20 elements and force 30
     表示在 0点10分，红魔1号武士iceman前进到1号城市，此时他生命值为20,攻击力为30
     对于iceman,输出的生命值和攻击力应该是变化后的数值

    4)武士放箭
     输出样例：
     000:35 blue dragon 1 shot
     表示在 0点35分，编号为1的蓝魔dragon武士射出一支箭。如果射出的箭杀死了敌人，
     则应如下输出：
     000:35 blue dragon 1 shot and killed red lion 4
     表示在 0点35分，编号为1的蓝魔dragon武士射出一支箭，杀死了编号为4的红魔lion。

    5)武士使用bomb
     输出样例：
     000:38 blue dragon 1 used a bomb and killed red lion 7
     表示在 0点38分，编号为1的蓝魔dragon武士用炸弹和编号为7的红魔lion同归于尽。

    6) 武士主动进攻
     输出样例：
     000:40 red iceman 1 attacked blue lion 1 in city 1 with 20 elements and force 30
     表示在0点40分，1号城市中，红魔1号武士iceman 进攻蓝魔1号武士lion，在发起进攻
     前，红魔1号武士iceman生命值为20，攻击力为 30

    7) 武士反击
     输出样例：
     001:40 blue dragon 2 fought back against red lion 2 in city 1
     表示在1点40分，1号城市中，蓝魔2号武士dragon反击红魔2号武士lion

    8) 武士战死
     输出样例：
     001:40 red lion 2 was killed in city 1
     被箭射死的武士就不会有这一条输出。

    9) 武士欢呼
     输出样例：
     003:40 blue dragon 2 yelled in city 4

    10) 武士获取生命元( elements )
     输出样例：
     001:40 blue dragon 2 earned 10 elements for his headquarter

    11) 旗帜升起
     输出样例：
     004:40 blue flag raised in city 4

    12) 武士抵达敌军司令部
     输出样例：
     001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
     (此时他生命值为20,攻击力为30）对于iceman,输出的生命值和攻击力应该是变化后的
     数值

    13) 司令部被占领
     输出样例：
     003:10 blue headquarter was taken

    14)司令部报告生命元数量
     000:50 100 elements in red headquarter
     000:50 120 elements in blue headquarter
     表示在0点50分，红方司令部有100个生命元，蓝方有120个

    15)武士报告武器情况
     000:55 blue wolf 2 has arrow(2),bomb,sword(23)
     000:55 blue wolf 4 has no weapon
     000:55 blue wolf 5 has sword(20)
     表示在0点55分，蓝魔2号武士wolf有一支arrow (这支arrow还可以用2次)，一个bomb，
     还有一支攻击力为23的sword。
     蓝魔4号武士wolf没武器。
     蓝魔5号武士wolf有一支攻击力为20的sword。
     交代武器情况时，次序依次是：arrow,bomb,sword。如果没有某种武器，某种武器就
     不用提。报告时，先按从西向东的顺序所有的红武士报告，然后再从西向东所有的蓝
     武士报告。

   输出事件时：
    首先按时间顺序输出；

    同一时间发生的事件，按发生地点从西向东依次输出. 武士前进的事件, 算是发生在
    目的地。

    在一次战斗中有可能发生上面的 6 至 11 号事件。这些事件都算同时发生，其时间就
    是战斗开始时间。一次战斗中的这些事件，序号小的应该先输出。

    两个武士同时抵达同一城市，则先输出红武士的前进事件，后输出蓝武士的。

    显然，13号事件发生之前的一瞬间一定发生了12号事件。输出时，这两件事算同一时
    间发生，但是应先输出12号事件

    虽然任何一方的司令部被占领之后，就不会有任何事情发生了。但和司令部被占领同
    时发生的事件，全都要输出。

   输入
    第一行是t,代表测试数据组数
    每组样例共三行。
    第一行，五个整数 M,N,R,K, T。其含义为：

    每个司令部一开始都有M个生命元( 1 <= M <= 10000)
    两个司令部之间一共有N个城市( 1 <= N <= 20 )
    arrow的攻击力是R
    lion每经过一场未能杀死敌人的战斗，忠诚度就降低K。
    要求输出从0时0分开始，到时间T为止(包括T) 的所有事件。T以分钟为单位，
    0 <= T <= 5000

    第二行：五个整数，依次是 dragon 、ninja、iceman、lion、wolf 的初始生命值。
    它们都大于0小于等于10000

    第三行：五个整数，依次是 dragon 、ninja、iceman、lion、wolf 的攻击力。它们
    都大于0小于等于10000

   输出
    对每组数据，先输出一行：
    Case n:
    如对第一组数据就输出 Case1:
    然后按恰当的顺序和格式输出到时间T为止发生的所有事件。每个事件都以事件发生的
    时间开头，时间格式是“时: 分”，“时”有三位，“分”有两位。

   样例输入
    1
    20 1 10 10 1000
    20 20 30 10 20
    5 5 5 5 5

   样例输出
    Case 1:
    000:00 blue lion 1 born
    Its loyalty is 10
    000:10 blue lion 1 marched to city 1 with 10 elements and force 5
    000:30 blue lion 1 earned 10 elements for his headquarter
    000:50 20 elements in red headquarter
    000:50 20 elements in blue headquarter
    000:55 blue lion 1 has no weapon
    001:00 blue dragon 2 born
    Its morale is 0.00
    001:10 blue lion 1 reached red headquarter with 10 elements and force 5
    001:10 blue dragon 2 marched to city 1 with 20 elements and force 5
    001:30 blue dragon 2 earned 10 elements for his headquarter
    001:50 20 elements in red headquarter
    001:50 10 elements in blue headquarter
    001:55 blue lion 1 has no weapon
    001:55 blue dragon 2 has arrow(3)
    002:10 blue dragon 2 reached red headquarter with 20 elements and force 5
    002:10 red headquarter was taken

 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <assert.h>

using namespace std;

#if !defined(nullptr)
  #define nullptr 0
#endif

static int debug;

//--------------------------------------------------------------------------

const int NUM_CITY = 20;
const int MIN_ELEMENTS = 1;
const int MAX_ELEMENTS = 10000;
const int MIN_ATTACKS = 1;
const int MAX_ATTACKS = 10000;

class City;
class Weapon;
class Warrior;
class HeadQuarter;

typedef vector<City> city_v;
typedef vector<Weapon*> weapon_v;
typedef vector<Warrior*> warrior_v;

const int NUM_HQ = 2;
enum flag_t                                     // 總部:紅軍,藍軍
    { _red_, _blue_, _none_ };
const char *FLAG_NAME[NUM_HQ+1] =
    { "red", "blue", "none" };

const int NUM_WARRIOR = 5;
enum warrior_t                                  // 武士:龍,忍者,冰人,獅,狼
    { _dragon_, _ninja_, _iceman_, _lion_, _wolf_ };
const char *WARRIOR_NAME[NUM_WARRIOR] =
    { "dragon", "ninja", "iceman", "lion", "wolf" };

const int NUM_WEAPON = 3;
enum weapon_t                                   // 武器:刀,炸彈,弓箭
    { _sword_, _bomb_, _arrow_ };
const char *WEAPON_NAME[NUM_WEAPON] =
    { "sword", "bomb", "arrow" };

inline flag_t Opponent(flag_t n) { return flag_t(1-n); }
inline flag_t Opponent(int n) { return flag_t(1-n); }
inline int Element2HP(int n) { return n; }      // 1個生命元 = 1個生命力

static int Warrior_Attacks[NUM_WARRIOR];        // input: 各武士初始攻擊力
static int Warrior_Elements[NUM_WARRIOR];       // input: 產生武士所需生命力

static int N;                                   // 有多少城市(不含總部)
static int R;                                   // 弓箭的固定攻擊力
static int K;                                   // 獅子的減少忠誠度

static int Hour;                                // 戰場時間: 時
static int Mins;                                // 分

static HeadQuarter *HQ[NUM_HQ];
static city_v BG;           // Battle Ground[]: 0:紅軍, 1..N:城市, N+1:藍軍

//--------------------------------------------------------------------------
static ostream& output_timer();
static ostream& output_HQ(const HeadQuarter *HQ);
static ostream& output_warrior(const Warrior *w);
static ostream& output_warrior(const Warrior *w, const string &action);
static Weapon * weapon_gen(Warrior *owner, int id);
//--------------------------------------------------------------------------

// 總部
class HeadQuarter {
  private:
    flag_t _flag;                       // red, blue
    int _source;                        // 總部所屬城市(red:0, blue:N+1)
    int _target;                        // 佔領目標城市(red:N+1, blue:0)
    bool _alive;
    int _elements;                      // 總部的生命元
    int _count;                         // number of warriors
    int _next_id;                       // index to _warrior_id[]
    int _warrior_id[NUM_WARRIOR];       // mapping to enum {dragon ... wolf}
    warrior_v _warriors;                // 武士物件指標陣列

  public:
    HeadQuarter(flag_t flag, int initElem, int source, int target, const int warrior_id[]);
    ~HeadQuarter();
    int count() const { return _count; }
    bool alive() const { return _alive; }
    flag_t flag() const { return _flag; }
    int source() const { return _source; }
    int target() const { return _target; }
    int elements() const { return _elements; }
    int move() const { return target() ? 1 : -1; }
    const char * name() const { return FLAG_NAME[_flag]; }
    const char * opponent_name() const { return FLAG_NAME[Opponent(_flag)]; }
    void add_elements(int n) { _elements += n; }
    void remove(Warrior *w);
    void remove_dead();
    void lost_battle();
    int generate();
    void report() const;
};

//--------------------------------------------------------------------------

// 城市
class City {
private:
    int _id;                            // 城市編號:1~N
    flag_t _flag;                       // none, red, blue
    flag_t _last_winner;                // 前次勝利者
    warrior_v _warriors;                // 紅藍武士物件指標陣列[2]
    int _elements;                      // 生命元
    int _occupied;                      // 城市(0,N+1)時代表總部被攻佔的次數
public:
    City(int id) : _id(id), _flag(_none_), _last_winner(_none_), _elements(0), _occupied(0) {
        _warriors.resize(NUM_HQ, nullptr);
    }
    flag_t flag() const { return _flag; }
    flag_t last_winner() const { return _last_winner; }
    int id() const { return _id; }
    int elements() const { return _elements; }
    int occupied() const { return _occupied; }
    int occupy() { return ++_occupied; }
    Warrior * warrior(flag_t n) const { return _warriors[n]; }
    void set_flag(flag_t value) { _flag = value; }
    void add_elements(int value) { _elements += value; }
    void take_away_elements();
    bool leave(Warrior *w);
    bool enter(Warrior *w);
    flag_t fight(flag_t attacker);
    bool fight_winner(flag_t winner) {
        bool changed = (winner != _none_ && winner != _flag &&
                        winner == _last_winner);
        if (changed) _flag = winner;
        _last_winner = winner;
        return changed;
    }
};

//--------------------------------------------------------------------------

// 武器
class Weapon {
  private:
    weapon_t _types;                    // 武器種類
    Warrior *_owner;                    // 擁有者
    int _atk;                           // 攻擊力
  public:
    Weapon(weapon_t types, Warrior *owner, int atk)
    : _types(types), _owner(owner), _atk(atk) {}
    const char * types_name() const { return WEAPON_NAME[_types]; }
    int atk() const { return _atk; };
    Warrior * owner() const { return _owner; }
    weapon_t types() const { return _types; }
    void set_atk(int n) { _atk = n; }
    void set_owner(Warrior *owner) { _owner = owner; }
    void change_owner(Warrior *new_owner);  // delay implement
    virtual int attack() { return _atk; }
    virtual string status() const { return types_name(); }
};

class Sword: public Weapon {
  public:
    Sword(Warrior *owner, int atk) : Weapon(_sword_, owner, atk) {}
    int attack() {      // 每次攻擊後會耗損80%
        int value = atk();
        set_atk(80 * value / 100);
        return value;
    }
    string status() const {
        ostringstream os;
        os << types_name() << '(' << atk() << ')';
        return os.str();
    }
};

class Arrow: public Weapon {
  private:
    int _count;                 // 可使用3次
  public:
    Arrow(Warrior *owner, int atk) : Weapon(_arrow_, owner, atk), _count(3) {}
    int attack() {
        if (_count <= 0) return 0;
        _count--;
        return atk();
    }
    string status() const {
        ostringstream os;
        os << types_name() << '(' << _count << ')';
        return os.str();
    }
};

class Bomb: public Weapon {
  public:
    Bomb(Warrior *owner, int atk) : Weapon(_bomb_, owner, atk) {}
    int attack() { return MAX_ATTACKS; }
};

//--------------------------------------------------------------------------

// 武士
class Warrior {
  private:
    HeadQuarter *_HQ;                   // red, blue
    int _id;
    warrior_t _species;                 // dragon, ninja, iceman, lion, wolf
    int _hp;                            // 生命值
    int _atk;                           // 攻擊力
    int _loyalty;                       // 忠誠度
    double _morale;                     // 士氣
    bool _fightback;                    // 受攻擊未陣亡時是否會反擊
    int _city;                          // 在那一城市
  protected:
    weapon_v _weapons;                  // 每種武器只能有1只, 依序:sword,bomb,arrow
  public:
    Warrior(HeadQuarter *HQ, int id, warrior_t nWarrior, int hp)
    : _HQ(HQ), _id(id), _species(nWarrior), _hp(hp),
      _loyalty(0), _morale(0), _fightback(true), _city(0) {
        _atk = Warrior_Attacks[nWarrior];
        _weapons.resize(NUM_WEAPON, nullptr);
    }
    virtual ~Warrior() {
        BG[_city].leave(this);
        for (int i = 0; i < _weapons.size(); i++) {
            delete _weapons[i];
        }
        _weapons.clear();
    }
    HeadQuarter * HQ() const { return _HQ; }
    flag_t flag() const { return _HQ->flag(); }
    int move() const { return _HQ->move(); }
    int id() const { return _id; }
    int species() const { return _species; }
    const char * species_name() const { return WARRIOR_NAME[_species]; }
    int atk() const { return _atk; }
    int loyalty() const { return _loyalty; }
    double morale() const { return _morale; }
    int city() const { return _city; }
    int target() const { return _HQ->target(); }
    int hp() const { return _hp; }
    int add_hp(int value) { return _hp += value; }
    bool dead() { return hp() <= 0; }
    bool fightback() const { return _fightback; }
    bool has_boom() { return _weapons[_bomb_] != nullptr; }
    void set_hp(int n) { _hp = n; }
    void set_atk(int n) { _atk = n; }
    void set_loyalty(int n) { _loyalty = n; }
    void set_morale(double n) { _morale = n; }
    void set_fightback(bool b) { _fightback = b; }
    void die() { _HQ->remove(this); }
    void disappear() { die(); }
    virtual bool run_away() { return false; }
    virtual void print_characters() {}
    virtual void cheer() const {}

    void set_city(int n) {
        if (n != _city) BG[_city].leave(this);
        _city = n;
        BG[_city].enter(this);
    }
    int sword_attack(bool fire) {
        Weapon *w = _weapons[_sword_];
        return w ? (fire ? w->attack() : w->atk()) : 0;
    }
    int fightback_attack(bool fire=true) {
        return _hp > 0 && _fightback ? _atk/2 + sword_attack(fire) : 0;
    }
    int forecast_attack(bool fire=true) {
        return _hp > 0 ? _atk + sword_attack(fire) : 0;
    }
    virtual int attack(Warrior *opponent) {
        // 先檢查雙方是否已無生命值, 可能在之前被對方弓箭射死, 延續算在本場戰鬥
        if (dead()) {
            return opponent->dead() ? 0 : -2;       // -1:自己已無生命值, 0:平局
        }
        if (opponent->dead()) return 2;             // 1:對手已無生命值

        // 這才開始戰鬥: 減少生命值及武器耗損
        int atk1 = forecast_attack();

        // (6) 武士主动进攻
        // 000:40 red iceman 1 attacked blue lion 1 in city 1 with 20 elements and force 30
        output_warrior(this) << "attacked "
            << opponent->species_name() << ' ' << opponent->id()
            << " in city " << city()
            << " with " << hp()
            << " elements and force " << atk()      // 是否要包含武器的攻擊力?
            << endl;

        if (opponent->add_hp(-atk1) > 0) {
            // 攻擊對手, 但未能殺死對方
            int atk2 = opponent->fightback_attack();// 對手反擊

            // (7) 武士反击
            // 001:40 blue dragon 2 fought back against red lion 2 in city 1 
            output_warrior(opponent) << "fought back against "
                << species_name() << ' ' << id()
                << " in city " << city()
                << endl;

            return add_hp(-atk2) <= 0 ? -1 : 0;         // -1:自己陣亡, 0:平局
        } else {
            // 對手為獅子死亡時將其生命值移轉到自己
            if (opponent->species() == _lion_) {
                add_hp(opponent->hp());
            }
            return 1;                               // 1:對手陣亡
        }
    }
    bool remove_weapon(Weapon *w) {
        weapon_t wt = w->types();
        if (_weapons[wt] == w) {
            _weapons[wt] = nullptr;
            return true;
        }
        return false;
    }
    bool add_weapon(Weapon *w) {
        weapon_t wt = w->types();
        if (_weapons[wt] == nullptr) {
            _weapons[wt] = w;
            return true;
        }
        return false;
    }
    void take_away_weapons(Warrior *opponent) { // 拿走對手的武器(自己沒有的)
        for (int i = 0; i < NUM_WEAPON; i++) {
            Weapon *w = opponent->_weapons[i];
            if (w != nullptr && _weapons[i] == nullptr) {
                _weapons[i] = w;
                opponent->_weapons[i] = nullptr;
            }
        }
    }
    virtual int forward() {
        if (_city == target()) return 0;
        City &C1 = BG[_city];
        C1.leave(this);

        if (debug) {
            cout << ">>> forward: city: " << _city
                << " to city: " << _city+move()
                << ", target: " << target()
                << endl;
        }

        _city += move();
        City &C2 = BG[_city];
        
        ostream &os = output_warrior(this);
        if (_city == target()) {
            // (12) 武士抵达敌军司令部
            // 001:10 red iceman 1 reached blue headquarter with 20 elements and force 30
            os << "reached " << _HQ->opponent_name() << " headquarter";
        } else {
            // (3) 武士前进到某一城市
            // 000:10 red iceman 1 marched to city 1 with 20 elements and force 30
            os << "marched to city " << _city;
        }
        os << " with " << hp() << " elements and force " << atk() << endl;

        if (_city == target()) {
            if (C2.occupy() >= 2) {
                flag_t opponent = Opponent(flag());
                ::HQ[opponent]->lost_battle();
            }
        } else {
            C2.enter(this);
        }
        return 1;
    }
    void earn_elements(int n) {
        _HQ->add_elements(n);
        // (10) 武士获取生命元
        // 001:40 blue dragon 2 earned 10 elements for his headquarter
        output_warrior(this) << "earned " << n
            << " elements for his headquarter" << endl;
    }
    string get_weapons() const {
        // has arrow(2),bomb,sword(23)
        // has no weapon
        ostringstream os;
        os << "has ";
        bool first = true;
        for (int i = _weapons.size(); --i >= 0;) {  // 指定倒序輸出:箭、炸彈、刀
            if (_weapons[i]) {
                if (first) first = false; else os << ',';
                os << _weapons[i]->status();
            }
        }
        if (first) os << "no weapon";
        return os.str();
    }
    void report() const {
        // (15) 武士报告武器情况
        // 000:55 blue wolf 2 has arrow(2),bomb,sword(23)
        output_warrior(this, get_weapons());
    }
};

// 龍
class Warrior_Dragon: public Warrior {
  public:
    Warrior_Dragon(HeadQuarter *HQ, int id, int hp, double morale)
    : Warrior(HQ, id, _dragon_, hp) {
        // 一件武器
        add_weapon(weapon_gen(this, id));
        // 士氣: 为它降生后其司令部剩余生命元的数量除以造dragon所需的生命元数量
        set_morale(morale);
    }
    void print_characters() {
        // Its morale is 23.34
        cout << "Its morale is "
            << fixed << setprecision(2) << morale()
            << endl;
    }
    void cheer() const {
        if (morale() > 0.8) {
            // (9) 武士欢呼
            // 003:40 blue dragon 2 yelled in city 4
            output_warrior(this) << "yelled in city " << city() << endl;
        }
    }
    int attack(Warrior *opponent) {
        int result = Warrior::attack(opponent);
        // 戰鬥獲勝士氣加0.2, 未能獲勝減0.2 (不考慮負值?), 死亡就不用管了
        if (result >= 0) {
            set_morale(morale() + (result > 0 ? 0.2 : -0.2));
        }
        return result;
    }
};

// 忍者
class Warrior_Ninja: public Warrior {
  public:
    Warrior_Ninja(HeadQuarter *HQ, int id, int hp)
    : Warrior(HQ, id, _ninja_, hp) {
        set_fightback(false);
        // 兩件武器
        add_weapon(weapon_gen(this, id));
        add_weapon(weapon_gen(this, id+1));
    }
};

// 冰人
class Warrior_Iceman: public Warrior {
  private:
    int _count;
  public:
    Warrior_Iceman(HeadQuarter *HQ, int id, int hp)
    : Warrior(HQ, id, _iceman_, hp) {
        // 一件武器
        add_weapon(weapon_gen(this, id));
        _count = 0;
    }
    int forward() {
        if (city() != target()) {
            // 每前進兩步，在第2步完成的時候，生命值會減少9，攻擊力會增加20
            // 若生命值減9後小於等於0，則生命值變為1 <-- 攻擊力還增加20嗎?
            // 即iceman不會因走多了而死
            _count++;
            if (_count >= 2) {
                _count = 0;
                if (add_hp(-9) <= 0) set_hp(1);
                set_atk(atk()+20);
            }
        }
        return Warrior::forward();
    }
};

// 獅子
class Warrior_Lion: public Warrior {
  public:
    Warrior_Lion(HeadQuarter *HQ, int id, int hp, int loyalty)
    : Warrior(HQ, id, _lion_, hp) {
        // 忠誠度: 等於它降生後其司令部剩餘生命元的數目
        set_loyalty(loyalty);
    }
    void print_characters() {
        // Its loyalty is 24
        cout << "Its loyalty is " << loyalty() << endl;
    }
    bool run_away() {
        return loyalty() <= 0;
    }
    // 每經過一場未能殺死敵人的戰鬥，忠誠度就降低K。忠誠度降至0或0以下，
    // 則該lion逃離戰場,永遠消失。但是已經到達敵人司令部的lion不會逃跑。
    // Lion在己方司令部可能逃跑。 lion 若是戰死，則其戰鬥前的生命值就會
    // 轉移到對手身上。所謂“戰鬥前”，就是每個小時的40分前的一瞬間。
    int attack(Warrior *opponent) {
        int result = Warrior::attack(opponent);
        if (result == 0) {
            set_loyalty(loyalty() - K);
        } else if (result < 0) {
            // 獅子死亡時將生命值移轉到對手
            opponent->add_hp(hp());
        }
        return result;
    }
};

// 狼
class Warrior_Wolf: public Warrior {
  public:
    Warrior_Wolf(HeadQuarter *HQ, int id, int hp)
    : Warrior(HQ, id, _wolf_, hp) {
        // 無特點
    }
    // 在戰鬥中如果獲勝(殺死敵人)，就會繳獲敵人的武器，但自己已有的武器就
    // 不繳獲了。被繳獲的武器不能算新的，已經被用到什麼樣了，就是什麼樣的。
    int attack(Warrior *opponent) {
        int result = Warrior::attack(opponent);
        if (result > 0) take_away_weapons(opponent);
        return result;
    }
};

//--------------------------------------------------------------------------

// class HeadQuarter; // 宣告放在前面, 會被其他class參考到

HeadQuarter::HeadQuarter(flag_t flag, int initElem, int source, int target, const int warrior_id[])
  : _flag(flag), _alive(true), _elements(initElem), _count(0),
    _next_id(0), _source(source), _target(target)
{
    std::copy(warrior_id, warrior_id+NUM_WARRIOR, _warrior_id);
}

HeadQuarter::~HeadQuarter()
{
    for (warrior_v::iterator it = _warriors.begin(); it != _warriors.end(); ++it) {
        delete *it;
    }
    _warriors.clear();
}

void HeadQuarter::remove(Warrior *w)
{
    for (warrior_v::iterator it = _warriors.begin(); it != _warriors.end(); ++it) {
        if (*it == w) {
            _warriors.erase(it);
            delete w;
            break;
        }
    }
}

void HeadQuarter::remove_dead()
{
    for (int i = _warriors.size(); --i >= 0;) {
        Warrior *w = _warriors[i];
        if (w->dead()) {
            _warriors.erase(_warriors.begin()+i);
            delete w;
        }
    }
}

void HeadQuarter::lost_battle()
{
    _alive = false;
    // (13) 司令部被占领
    // 003:10 blue headquarter was taken
    output_timer() << name() << " headquarter was taken" << endl;
}

int HeadQuarter::generate()
{
    // 如果司令部中的生命元不足以制造某武士，那么司令部就等待，直到获得足够
    // 生命元后的第一个整点，才制造该武士。
    int wid = _warrior_id[_next_id];
    int nElem = Warrior_Elements[wid];
    if (_elements < nElem) return -1;

    _next_id = (_next_id + 1) % NUM_WARRIOR;
    _elements -= nElem;
    _count++;

    int nHP = Element2HP(nElem);
    Warrior *pw;
    switch(wid) {
      case _dragon_:
        // 士氣: 为它降生后其司令部剩余生命元的数量除以造dragon所需的生命元数量
        pw = new Warrior_Dragon(this, _count, nHP, (double)_elements/nElem);
        break;
      case _ninja_:
        pw = new Warrior_Ninja(this, _count, nHP);
        break;
      case _iceman_:
        pw = new Warrior_Iceman(this, _count, nHP);
        break;
      case _lion_:
        // 忠诚度: 等于它降生后其司令部剩余生命元的数目
        pw = new Warrior_Lion(this, _count, nHP, _elements);
        break;
      case _wolf_:
        pw = new Warrior_Wolf(this, _count, nHP);
        break;
    }

    pw->set_city(_source);
    _warriors.push_back(pw);

    // (1) 武士降生
    // 000:00 blue lion 1 born
    output_warrior(pw, "born");
    pw->print_characters();

    return wid;
}

void HeadQuarter::report() const
{
    // (14) 司令部报告生命元数量
    // 000:50 100 elements in red headquarter
    output_timer() << elements() << " elements in "
        << name() << " headquarter" << endl;
}

//--------------------------------------------------------------------------

// 延後定義: 因為會使用到 Weapon::atk()
static Weapon * weapon_gen(Warrior *owner, int id)
{
    switch(id % NUM_WEAPON) {
      case _sword_: // 擁有武士20%的攻擊力
        return new Sword(owner, owner->atk()/5);
      case _bomb_:
        return new Bomb(owner, 0);
      case _arrow_: // 攻擊力固定為R(輸入)
        return new Arrow(owner, R);
    }
    assert(0);  // impossible!
}

// 延後定義: 因為會用到 Warrior::{add,remove}_weapon()
void Weapon::change_owner(Warrior *new_owner)
{
    if (_owner) _owner->remove_weapon(this);
    _owner = new_owner;
    if (_owner) _owner->add_weapon(this);
}

// 延後定義: 因為會用到 Warrior 類的內容

// 如果某个城市中只有一个武士，那么该武士取走该城市中的所有生命元，
// 并立即将这些生命元传送到其所属的司令部
void City::take_away_elements()
{
    if (_elements <= 0) return;
    for (flag_t n = _red_; n != _none_; n = flag_t(n+1)) {
        if (_warriors[n] != nullptr && _warriors[Opponent(n)] == nullptr) {
            _warriors[n]->earn_elements(_elements);
            _elements = 0;
            break;
        }
    }
}

// 每一城市同時最多只有紅方或藍方各一名武士
// 武士出城(到下一城市、死亡、逃走消失)
bool City::leave(Warrior *w)
{
    int n = w->flag();
    if (_warriors[n] == w) {
        _warriors[n] = nullptr;
        return true;
    }
    return false;
}

// 武士進城
bool City::enter(Warrior *w)
{
    int n = w->flag();
    if (_warriors[n] != w) {
        _warriors[n] = w;
        return true;
    }
    return false;
}

// 城市中雙方各有一個武士則會發生戰鬥, 連續贏得2次即可插上己方的旗子
flag_t City::fight(flag_t attacker)
{
    assert(attacker == _red_ || attacker == _blue_);
    flag_t winner = _none_;

    if (_warriors[_red_] && _warriors[_blue_]) {
        flag_t opponent = Opponent(attacker);
        int result = _warriors[attacker]->attack(_warriors[opponent]);
        if (result) {
            winner = result > 0 ? attacker : opponent;
            if (result & 1) {   // 戰鬥獲勝, 而非對手被己方的弓箭射死的
                // (8) 武士战死
                // 001:40 red lion 2 was killed in city 1
                output_warrior(_warriors[Opponent(winner)])
                    << "was killed in city " << id() << endl;
            }

            Warrior *w = _warriors[winner];

            // (9) 武士欢呼
            w->cheer();

            if (_elements > 0) {
                // (10) 武士获取生命元
                w->earn_elements(_elements);
                _elements = 0;
            }
        }

        if (fight_winner(winner)) {
            // (11) 旗帜升起
            // 输出样例：004:40 blue flag raised in city 4
            output_HQ(HQ[winner]) << "flag raised in city " << id() << endl;
        }
    }

    return winner;
}

static ostream& output_timer()
{
    return cout << setfill('0') << setw(3) << Hour
        << ':' << setw(2) << Mins
        << ' ';
}

static ostream& output_HQ(const HeadQuarter *HQ)
{
    return output_timer() << HQ->name() << ' ';
}

static ostream& output_warrior(const Warrior *w)
{
    return output_HQ(w->HQ()) << w->species_name() << ' ' << w->id() << ' ';
}

static ostream& output_warrior(const Warrior *w, const string &action)
{
    return output_warrior(w) << action << endl;
}

//--------------------------------------------------------------------------

static void run_away()
{
    // 不包含兩端總部
    for (int i = 1; i <= N; i++) {
        for (flag_t n = _red_; n != _none_; n = flag_t(n+1)) {
            Warrior *w = BG[i].warrior(n);
            if (w && w->run_away()) {
                // (2) lion逃跑
                // 000:05 blue lion 1 ran away
                output_warrior(w, "ran away");
                w->disappear();
            }
        }
    }
}

// 武士前进的事件, 算是发生在目的地
static void backward_check(flag_t who, int n)
{
    if (0 <= n && n < BG.size()) {
        Warrior *w = BG[n].warrior(who);
        if (w) w->forward();
    }
}

static void forward()
{
    int red_move = HQ[_red_]->move();
    int blue_move = HQ[_blue_]->move();

    // 要包含兩端總部
    for (int i = 0; i < BG.size(); i++) {
        backward_check(_red_, i-red_move);
        backward_check(_blue_, i-blue_move);
    }
}

// 每个城市产出10个生命元
static void add_city_elements(int elem)
{
    for (int i = 1; i <= N; i++) {
        BG[i].add_elements(elem);
    }
}

// 如果某个城市中只有一个武士，那么该武士取走该城市中的所有生命元，
// 并立即将这些生命元传送到其所属的司令部
static void take_away_city_elements()
{
    for (int i = 1; i <= N; i++) {
        BG[i].take_away_elements();
    }
}

// 在插红旗的城市，以及编号为奇数的无旗城市，由红武士主动发起进攻。
// 在插蓝旗的城市，以及编号为偶数的无旗城市，由蓝武士主动发起进攻。
// 戰鬥規則參見 line 28~48 說明
// 戰鬥完後回收死亡的戰士及其武器
static void launch_city_fight()
{
    int RElems = HQ[_red_]->elements();
    int BElems = HQ[_blue_]->elements();
    vector<flag_t> flags(N+1, _none_);      // 記錄每一城市誰贏

    for (int i = 1; i <= N; i++) {
        City &C = BG[i];
        flag_t attacker = C.flag();
        if (attacker == _none_) attacker = i & 1 ? _red_ : _blue_;
        // 記錄這個城市誰贏, 事後再算分數
        flags[i] = C.fight(attacker);
    }

/*
    如果武士在战斗中杀死敌人 (不论是主动进攻杀死还是反击杀死)，则其司令部会立即
    向其发送8个生命元作为奖励，使其生命值增加8。当然前提是司令部得有8个生命元。
    如果司令部的生命元不足以奖励所有的武士，则优先奖励距离敌方司令部近的武士。
*/
    // 紅方發放獎勵
    int nElems = 0;
    for (int i = N; i >= 0 && nElems < RElems; i--) {
        if (flags[i] == _red_) {
            Warrior *w = BG[i].warrior(_red_);
            assert(w != nullptr);
            int n = min(8, RElems-nElems);
            w->add_hp(n);
            nElems += n;
        }
    }
    HQ[_red_]->add_elements(-nElems);

    // 藍方發放獎勵
    nElems = 0;
    for (int i = 0; i <= N && nElems < BElems; i++) {
        if (flags[i] == _blue_) {
            Warrior *w = BG[i].warrior(_blue_);
            assert(w != nullptr);
            int n = min(8, BElems-nElems);
            w->add_hp(n);
            nElems += n;
        }
    }
    HQ[_blue_]->add_elements(-nElems);

    // 回收死亡戰士
    HQ[_red_]->remove_dead();
    HQ[_blue_]->remove_dead();
}

// 在每个小时的第35分，拥有arrow的武士放箭，对敌人造成伤害。放箭事件应算发生
// 在箭发出的城市。注意，放箭不算是战斗，因此放箭的武士不会得到任何好处。武士
// 在没有敌人的城市被箭射死也不影响其所在城市的旗帜更换情况。
static void launch_arrow_shoot()
{
}

static void launch_bomb()
{
}

static void report_warriors(flag_t n)
{
    // (15) 武士报告武器情况
    for (int i = 1; i <= N; i++) {
        Warrior *w = BG[i].warrior(n);
        if (w) w->report();
    }
}

static void run_case(int elements, int mins)
{
    BG.reserve(N+2);
    for (int i = 0; i < N+2; i++) {
        BG.push_back(City(i));
    }

    const int red_id[] = { _iceman_, _lion_, _wolf_, _ninja_, _dragon_ };
    HeadQuarter redHQ(_red_, elements, 0, N+1, red_id);

    const int blue_id[] = { _lion_, _dragon_, _ninja_, _iceman_, _wolf_ };
    HeadQuarter blueHQ(_blue_, elements, N+1, 0, blue_id);

    HQ[_red_] = &redHQ;
    HQ[_blue_] = &blueHQ;

    if (debug) {
        cout << "Time: " << mins
            << ", Cities: " << N
            << ", Elements: " << elements
            << endl;
    }

/*
   输出事件时：
    首先按时间顺序输出；

    同一时间发生的事件，按发生地点从西向东依次输出. 武士前进的事件, 算是发生在
    目的地。(即指 City1...CityN)

    在一次战斗中有可能发生上面的 6 至 11 号事件。这些事件都算同时发生，其时间就
    是战斗开始时间。一次战斗中的这些事件，序号小的应该先输出。

    两个武士同时抵达同一城市，则先输出红武士的前进事件，后输出蓝武士的。

    显然，13号事件发生之前的一瞬间一定发生了12号事件。输出时，这两件事算同一时
    间发生，但是应先输出12号事件

    虽然任何一方的司令部被占领之后，就不会有任何事情发生了。但和司令部被占领同
    时发生的事件，全都要输出。
 */    
    int n = 0;
    while (n < mins && redHQ.alive() && blueHQ.alive()) {
        Hour = n / 60;
        Mins = n % 60;
        switch(Mins) {
        case 0: // 制造武士
            redHQ.generate();
            blueHQ.generate();
            n += 5;
            break;
        case 5: // 该逃跑的lion就在这一时刻逃跑了
            run_away();
            n += 5;
            break;
        case 10: // 所有的武士朝敌人司令部方向前进一步
            forward();
            n += 10;
            break;
        case 20: // 每个城市产出10个生命元
            add_city_elements(10);
            n += 10;
            break;
        case 30: // 如果某个城市中只有一个武士，那么该武士取走该城市中的
                 // 所有生命元，并立即将这些生命元传送到其所属的司令部
            take_away_city_elements();
            n += 5;
            break;
        case 35: // 拥有arrow的武士放箭，对敌人造成伤害...
                 // 暫不回收死亡者, 在40分的戰鬥結果後才收回
            launch_arrow_shoot();
            n += 3;
            break;
        case 38: //拥有bomb的武士评估是否应该使用bomb。如果是，
                 // 就用bomb和敌人同归于尽
            launch_bomb();
            n += 2;
            break;
        case 40: // 在有两个武士的城市，会发生战斗...
            launch_city_fight();
            n += 10;
            break;
        case 50: // 司令部报告它拥有的生命元数量
            redHQ.report();
            blueHQ.report();
            n += 5;
            break;
        case 55: // 每个武士报告其拥有的武器情况
            report_warriors(_red_);
            report_warriors(_blue_);
            n += 5;
            break;
        }
    }

    BG.clear();
}

static void run()
{
    int num_case;
    cin >> num_case;
    for (int n = 1; n <= num_case; n++) {
        int M;  // 司令部一開始的生命元
      //int N;  // 多少城市
      //int R;  // 弓箭攻擊力(全域變數)
      //int K;  // 獅子忠誠度遞減值(全域變數)
        int T;  // 多少分鐘

        cin >> M >> N >> R >> K >> T;
        assert(0 <= M && M <= MAX_ELEMENTS);
        for (int i = 0; i < NUM_WARRIOR; i++) {
            int v;
            cin >> v;
            assert(MIN_ELEMENTS <= v && v <= MAX_ELEMENTS);
            Warrior_Elements[i] = v;
        }
        for (int i = 0; i < NUM_WARRIOR; i++) {
            int v;
            cin >> v;
            assert(MIN_ATTACKS <= v && v <= MAX_ATTACKS);
            Warrior_Attacks[i] = v;
        }

        cout << "Case " << n << ':' << endl;
        run_case(M, T);
    }
}

int main (int argc, char *argv[])
{
    int n = 1;
    while (n < argc && argv[n][0] == '-') {
        if (strncmp(argv[n], "-d", 2) == 0) {
            debug += strlen(&argv[n][1]);
        }
        n++;
    }

    if (n < argc) {
        while (n < argc) {
            ifstream in(argv[n]);
            //streambuf *cinbuf = cin.rdbuf(); //save old buf
            cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
            run();
            n++;
        }
    } else {
        run();
    }

    return 0;
}

