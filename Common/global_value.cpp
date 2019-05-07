/*
	(C) 2012-2019  Tianyi Hao
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
	*/

#include "..\Headers\game.h"

int board[N][N];
int bmove;

int nosix = 0;
long long int timee = 30000000;    // ����ʱ��
volatile long long int times = 0;         // Ԥ��ʱ��
long long int timel = 30000000;     // �ܾ�ʱ
long long int timestart = 0;
long long int nodelimit = 0;
const char version[] = "5.1.3";
volatile long long int ts;
volatile int compend = 0;
volatile int comphalfend = 0;
int staticboard = 1;
int movei;
int fflag = 0;
#ifndef ANDROID
int S;
int boardSize;
#else
int S = 15;
int boardSize = 15;
#endif
int computevcf = 1;
volatile int thinklock = 0;
int moven = 0;
int nbest = 0;

double para[] = {
	-0.200199873065,
	-0.170322649129,
	-0.221213772278, -0.145485105835,
	-0.129801153017, -0.0941755741609, 0.0,
	-0.131349267488, -0.142689532514, -0.0632002176032, 0.0,
	0.208204632758, 0.192041324021, 0.344950087283, 0.519204612068, 1.10883482175,
	0.145252469294, 0.216702781034, 0.48553211621, 0.341190876205, 1.04571622352, 1.51813373132,
	0.174007346864, 0.221036836655, 0.39655640299, 0.296433610804, 1.30125748399, 1.19273563816, 2.33642075431,
	0.157288859627, 0.204367331693, 0.240748254531, 0.355117034417, 1.22669466803, 0.848066675193, 2.24389849244, 3.20704488372,
	4.17958823351, 5.20719869843, 3.08814999105, 3.93385613475, 4.68113882153, 6.43325766319, 5.33315273998, 6.24419627379, 13.7716552587,
	3.56866577915, 5.42407581362, 3.2903070389, 3.48338257379, 5.82273683946, 5.48852718152, 6.2336566946, 7.01511929977, 14.0358529407, 18.0361368954,
	5.54417410876, 7.35062547833, 5.8642947218, 7.73517059503, 6.60659581115, 9.6249111049, 8.20981207734, 15.448889251, 20.9492603746, 28.1239918101, 30.3911258288,
	4.53906477086, 7.17485032762, 7.16285680069, 7.73826590139, 15.3394421418, 10.0721208528, 15.0627547177, 16.6105497107, 21.5580486148, 27.5164715535, 45.1945286391, 80.7976397982,
	33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0, 33554432.0,
	-0.200199873065,
	-0.170322649129,
	-0.221213772278, -0.145485105835,
	-0.129801153017, -0.0941755741609, 0.0,
	-0.131349267488, -0.142689532514, -0.0632002176032, 0.0,
	0.208204632758, 0.192041324021, 0.344950087283, 0.519204612068, 1.10883482175,
	0.145252469294, 0.216702781034, 0.48553211621, 0.341190876205, 1.04571622352, 1.51813373132,
	0.174007346864, 0.221036836655, 0.39655640299, 0.296433610804, 1.30125748399, 1.19273563816, 2.33642075431,
	0.157288859627, 0.204367331693, 0.240748254531, 0.355117034417, 1.22669466803, 0.848066675193, 2.24389849244, 3.20704488372,
	4.17958823351, 5.20719869843, 3.08814999105, 3.93385613475, 4.68113882153, 6.43325766319, 5.33315273998, 6.24419627379, 39.4294714723,
	3.56866577915, 5.42407581362, 3.2903070389, 3.48338257379, 5.82273683946, 5.48852718152, 6.2336566946, 7.01511929977, 21.603021827, 37.867512837,
	5.54417410876, 7.35062547833, 5.8642947218, 7.73517059503, 6.60659581115, 9.6249111049, 8.20981207734, 15.448889251, 65575.5, 65575.5, 1048576.0,
	1048576.0, 1048576.0, 1048576.0, 1048576.0, 1048576.0, 1048576.0, 1048576.0, 1048576.0, 1048576.0, 1048576.0, 1048576.0, 1048576.0,
	268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0, 268435456.0,
	-100000.0,
	0.0,
	0.0, 0.0,
	0.886018420768, 0.835583279643, 1.77416498804,
	0.644384362664, 0.801531573897, 1.77487493802, 1.95960534715,
	3.14829532507, 3.69020583412, 5.63643933469, 4.52412071951, 11.2445732238,
	4.52593109195, 4.52231107122, 5.01550588021, 6.00119235596, 10.5808413095, 11.5075404339,
	3.93222149067, 5.0029948279, 6.00119235597, 6.63641316835, 5.34495803693, 9.23659406521, 13.0269373109,
	11.5167519928, 11.4983362428, 9.78135923365, 13.4545978092, 12.4241245661, 10.4709104162, 12.2978256994, 17.7274593992,
	12.2522322316, 14.9533395536, 9.81891435109, 14.6606065128, 12.8296370372, 15.6896506552, 17.329573927, 20.7435085702, 102.020270051,
	13.5382828635, 14.9473582178, 16.2124185239, 15.5766374054, 16.6497080724, 23.4449464221, 24.3726124625, 21.175315606, 72.7425529171, 108.264726741,
	15.3694537031, 11.3923505329, 10.5249445902, 14.489014069, 24.4531199456, 18.9937064991, 17.9268596202, 31.7636297707, 3000.0, 3000.0, 4000.0,
	6000.0, 6000.0, 6000.0, 6000.0, 6000.0, 6000.0, 6000.0, 6000.0, 6000.0, 6000.0, 6000.0, 6000.0,
	1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0, 1000000.0,
	85.1272600349,
	0.0,
	0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	1.12773896252, 1.06141888961, 0.940626086705, 1.08438123666, 2.12623771894,
	1.08438123666, 1.27205129956, 0.804101097918, 1.00220152616, 1.54389958192, 2.92939772038,
	1.15029374177, 1.29801153017, 0.820183119877, 1.17611948218, 2.25728339059, 1.7025452007, 1.39835741863,
	1.19580838794, 1.29853094255, 0.853659981912, 0.771332354646, 1.88652811763, 2.00039745199, 2.21302293195, 1.03402391629,
	6.00599619195, 5.66184909253, 6.01080387333, 4.35757407881, 8.54148848526, 5.84206686515, 9.85081782442, 5.84674332432, 20.8343072983,
	6.09757129937, 5.29133995285, 6.99579108931, 5.52275819098, 7.54913212337, 10.3882475404, 8.49814827583, 8.15835695547, 21.1399715376, 26.7968415257,
	11.2492008039, 13.1644216983, 11.469595146, 8.15844975155, 13.5328675503, 10.0310117604, 13.7980034913, 15.8876611293, 1200.0, 1201.0, 1400.0,
	1500.0, 1500.0, 1500.0, 1500.0, 1501.0, 1501.0, 1501.0, 1501.0, 1506.0, 1507.0, 1511.0, 2000.0,
	50000.0, 50000.0, 50000.0, 50000.0, 50000.0, 50000.0, 50000.0, 50000.0, 50000.0, 50000.0, 50000.0, 50000.0, 50000.0,
	18.8803824614,
	0.547279584421,
	2.35412188522,
	3.67237307702,
	0.0,
	0.0,
	0.9
};
