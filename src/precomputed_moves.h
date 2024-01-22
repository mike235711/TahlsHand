#include <array>
#include <map>

#ifndef PRECOMPUTED_MOVES_H
#define PRECOMPUTED_MOVES_H
// Notes:
// Typically, constexpr variables are placed in read-only sections of memory,
// which can be more cache-friendly and potentially lead to faster access.
// Therefore we should try to make the precomputedBishopMovesTable and
// precomputedRookMovesTable constexpr.

// inline keyword is used to replace everytime there's a mention of the variable
// with the variable definition. However the compiler can choose to not replace it,
// since if the replacement is too big the performance might decrease at runtime.

namespace precomputed_moves
{
    constexpr std::array<uint64_t, 64> one_one_bits{1, 2, 4, 8, 16, 32, 64, 128,
                                                    256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 
                                                    65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 
                                                    16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 
                                                    4294967296, 8589934592, 17179869184, 34359738368, 68719476736, 137438953472, 274877906944, 549755813888, 
                                                    1099511627776, 2199023255552, 4398046511104, 8796093022208, 17592186044416, 35184372088832, 70368744177664, 140737488355328, 
                                                    281474976710656, 562949953421312, 1125899906842624, 2251799813685248, 4503599627370496, 9007199254740992, 18014398509481984, 36028797018963968, 
                                                    72057594037927936, 144115188075855872, 288230376151711744, 576460752303423488, 1152921504606846976, 2305843009213693952, 4611686018427387904, 9223372036854775808};

    constexpr uint64_t getBishopValidMovesIncludingCaptures(unsigned short square, uint64_t blockers_bit) // Works
    // Given a square and a blockers bit get the bitboard representing the moveable squares including blocker capture
    {
        uint64_t moves_bit{};
        int r = square / 8;
        int c = square % 8;
        int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}; // Diagonals
        for (int i = 0; i < 4; ++i)                                  // For each direction
        {
            int dr = directions[i][0];
            int dc = directions[i][1];
            int nr = r + dr;
            int nc = c + dc;

            while (0 <= nr && nr <= 7 && 0 <= nc && nc <= 7)
            {
                uint64_t pos_bit = 1ULL << (nr * 8 + nc);
                moves_bit |= pos_bit;

                if ((blockers_bit & pos_bit) != 0)
                {
                    // If there's a blocker, include this square as a capture but stop looking further in this direction.
                    break;
                }

                nr += dr;
                nc += dc;
            }
        }
        return moves_bit;
    }

    constexpr uint64_t getRookValidMovesIncludingCaptures(unsigned short square, uint64_t blockers_bit) // Works
    // Given a square and a blockers bit get the bitboard representing the moveable squares including blocker capture
    {
        uint64_t moves_bit{};
        int r = square / 8;
        int c = square % 8;
        int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}; // Diagonals
        for (int i = 0; i < 4; ++i)                                // For each direction
        {
            int dr = directions[i][0];
            int dc = directions[i][1];
            int nr = r + dr;
            int nc = c + dc;

            while (0 <= nr && nr <= 7 && 0 <= nc && nc <= 7)
            {
                uint64_t pos_bit = 1ULL << (nr * 8 + nc);
                moves_bit |= pos_bit;

                if ((blockers_bit & pos_bit) != 0)
                {
                    // If there's a blocker, include this square as a capture but stop looking further in this direction.
                    break;
                }

                nr += dr;
                nc += dc;
            }
        }
        return moves_bit;
    }

    constexpr std::array<std::array<uint64_t, 64>,64> getBishopOneBlockerTable() // Works
    {
        std::array<std::array<uint64_t, 64>, 64> table{};
        for (int i = 0; i < 64; ++i)
        {
            for (int j = 0; j < 64; ++j)
            {
                // We compute a ray from i to j including i and excluding j
                if (i == j)
                    table[i][j] = 0;
                else
                    table[i][j] = ((getBishopValidMovesIncludingCaptures(i, one_one_bits[j]) &
                                getBishopValidMovesIncludingCaptures(j, one_one_bits[i])) |
                               one_one_bits[i]);
            }
        }
        return table;
    }
    constexpr std::array<std::array<uint64_t, 64>, 64> getRookOneBlockerTable() // Works
    {
        std::array<std::array<uint64_t, 64>, 64> table{};
        for (int i = 0; i < 64; ++i)
        {
            for (int j = 0; j < 64; ++j)
            {
                // We compute a ray from i to j including i and excluding j
                if (i == j)
                    table[i][j] = 0;
                else
                    table[i][j] = ((getRookValidMovesIncludingCaptures(i, one_one_bits[j]) &
                                getRookValidMovesIncludingCaptures(j, one_one_bits[i])) |
                               one_one_bits[i]);
            }
        }
        return table;
    }
    // Initialize precomputed move tables (generated using my python move generator)
    inline constexpr std::array<uint64_t, 64> knight_moves = {132096, 329728, 659712, 1319424, 2638848, 5277696, 10489856, 4202496, 33816580, 84410376, 168886289, 337772578, 675545156, 1351090312, 2685403152, 1075839008, 8657044482, 21609056261, 43234889994, 86469779988, 172939559976, 345879119952, 687463207072, 275414786112, 2216203387392, 5531918402816, 11068131838464, 22136263676928, 44272527353856, 88545054707712, 175990581010432, 70506185244672, 567348067172352, 1416171111120896, 2833441750646784, 5666883501293568, 11333767002587136, 22667534005174272, 45053588738670592, 18049583422636032, 145241105196122112, 362539804446949376, 725361088165576704, 1450722176331153408, 2901444352662306816, 5802888705324613632, 11533718717099671552, 4620693356194824192, 288234782788157440, 576469569871282176, 1224997833292120064, 2449995666584240128, 4899991333168480256, 9799982666336960512, 1152939783987658752, 2305878468463689728, 1128098930098176, 2257297371824128, 4796069720358912, 9592139440717824, 19184278881435648, 38368557762871296, 4679521487814656, 9077567998918656};
    inline constexpr std::array<uint64_t, 64> king_moves = {770, 1797, 3594, 7188, 14376, 28752, 57504, 49216, 197123, 460039, 920078, 1840156, 3680312, 7360624, 14721248, 12599488, 50463488, 117769984, 235539968, 471079936, 942159872, 1884319744, 3768639488, 3225468928, 12918652928, 30149115904, 60298231808, 120596463616, 241192927232, 482385854464, 964771708928, 825720045568, 3307175149568, 7718173671424, 15436347342848, 30872694685696, 61745389371392, 123490778742784, 246981557485568, 211384331665408, 846636838289408, 1975852459884544, 3951704919769088, 7903409839538176, 15806819679076352, 31613639358152704, 63227278716305408, 54114388906344448, 216739030602088448, 505818229730443264, 1011636459460886528, 2023272918921773056, 4046545837843546112, 8093091675687092224, 16186183351374184448, 13853283560024178688, 144959613005987840, 362258295026614272, 724516590053228544, 1449033180106457088, 2898066360212914176, 5796132720425828352, 11592265440851656704, 4665729213955833856};
    inline constexpr std::array<uint64_t, 64> white_pawn_moves = {0, 0, 0, 0, 0, 0, 0, 0, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296, 8589934592, 17179869184, 34359738368, 68719476736, 137438953472, 274877906944, 549755813888, 1099511627776, 2199023255552, 4398046511104, 8796093022208, 17592186044416, 35184372088832, 70368744177664, 140737488355328, 281474976710656, 562949953421312, 1125899906842624, 2251799813685248, 4503599627370496, 9007199254740992, 18014398509481984, 36028797018963968, 72057594037927936, 144115188075855872, 288230376151711744, 576460752303423488, 1152921504606846976, 2305843009213693952, 4611686018427387904, 9223372036854775808, 0, 0, 0, 0, 0, 0, 0, 0};
    inline constexpr std::array<uint64_t, 64> white_pawn_attacks = {512, 1280, 2560, 5120, 10240, 20480, 40960, 16384, 131072, 327680, 655360, 1310720, 2621440, 5242880, 10485760, 4194304, 33554432, 83886080, 167772160, 335544320, 671088640, 1342177280, 2684354560, 1073741824, 8589934592, 21474836480, 42949672960, 85899345920, 171798691840, 343597383680, 687194767360, 274877906944, 2199023255552, 5497558138880, 10995116277760, 21990232555520, 43980465111040, 87960930222080, 175921860444160, 70368744177664, 562949953421312, 1407374883553280, 2814749767106560, 5629499534213120, 11258999068426240, 22517998136852480, 45035996273704960, 18014398509481984, 144115188075855872, 360287970189639680, 720575940379279360, 1441151880758558720, 2882303761517117440, 5764607523034234880, 11529215046068469760, 4611686018427387904, 0, 0, 0, 0, 0, 0, 0, 0};
    inline constexpr std::array<uint64_t, 64> black_pawn_moves = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 4294967296, 8589934592, 17179869184, 34359738368, 68719476736, 137438953472, 274877906944, 549755813888, 1099511627776, 2199023255552, 4398046511104, 8796093022208, 17592186044416, 35184372088832, 70368744177664, 140737488355328, 0, 0, 0, 0, 0, 0, 0, 0};
    inline constexpr std::array<uint64_t, 64> black_pawn_attacks = {0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 10, 20, 40, 80, 160, 64, 512, 1280, 2560, 5120, 10240, 20480, 40960, 16384, 131072, 327680, 655360, 1310720, 2621440, 5242880, 10485760, 4194304, 33554432, 83886080, 167772160, 335544320, 671088640, 1342177280, 2684354560, 1073741824, 8589934592, 21474836480, 42949672960, 85899345920, 171798691840, 343597383680, 687194767360, 274877906944, 2199023255552, 5497558138880, 10995116277760, 21990232555520, 43980465111040, 87960930222080, 175921860444160, 70368744177664, 562949953421312, 1407374883553280, 2814749767106560, 5629499534213120, 11258999068426240, 22517998136852480, 45035996273704960, 18014398509481984};
    inline constexpr std::array<uint64_t, 64> white_pawn_doubles = {0, 0, 0, 0, 0, 0, 0, 0, 16777216, 33554432, 67108864, 134217728, 268435456, 536870912, 1073741824, 2147483648, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    inline constexpr std::array<uint64_t, 64> black_pawn_doubles = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4294967296, 8589934592, 17179869184, 34359738368, 68719476736, 137438953472, 274877906944, 549755813888, 0, 0, 0, 0, 0, 0, 0, 0};
    // Moveable squares bitboard for bishop and rook without taking into account the edge squares
    // Used for computing blocker_bits given a position, used in BitPosition class
    inline constexpr std::array<uint64_t, 64> rook_unfull_rays = {282578800148862, 565157600297596, 1130315200595066, 2260630401190006, 4521260802379886, 9042521604759646, 18085043209519166, 36170086419038334, 282578800180736, 565157600328704, 1130315200625152, 2260630401218048, 4521260802403840, 9042521604775424, 18085043209518592, 36170086419037696, 282578808340736, 565157608292864, 1130315208328192, 2260630408398848, 4521260808540160, 9042521608822784, 18085043209388032, 36170086418907136, 282580897300736, 565159647117824, 1130317180306432, 2260632246683648, 4521262379438080, 9042522644946944, 18085043175964672, 36170086385483776, 283115671060736, 565681586307584, 1130822006735872, 2261102847592448, 4521664529305600, 9042787892731904, 18085034619584512, 36170077829103616, 420017753620736, 699298018886144, 1260057572672512, 2381576680245248, 4624614895390720, 9110691325681664, 18082844186263552, 36167887395782656, 35466950888980736, 34905104758997504, 34344362452452352, 33222877839362048, 30979908613181440, 26493970160820224, 17522093256097792, 35607136465616896, 9079539427579068672, 8935706818303361536, 8792156787827803136, 8505056726876686336, 7930856604974452736, 6782456361169985536, 4485655873561051136, 9115426935197958144};
    inline constexpr std::array<uint64_t, 64> bishop_unfull_rays = {18049651735527936, 70506452091904, 275415828992, 1075975168, 38021120, 8657588224, 2216338399232, 567382630219776, 9024825867763712, 18049651735527424, 70506452221952, 275449643008, 9733406720, 2216342585344, 567382630203392, 1134765260406784, 4512412933816832, 9024825867633664, 18049651768822272, 70515108615168, 2491752130560, 567383701868544, 1134765256220672, 2269530512441344, 2256206450263040, 4512412900526080, 9024834391117824, 18051867805491712, 637888545440768, 1135039602493440, 2269529440784384, 4539058881568768, 1128098963916800, 2256197927833600, 4514594912477184, 9592139778506752, 19184279556981248, 2339762086609920, 4538784537380864, 9077569074761728, 562958610993152, 1125917221986304, 2814792987328512, 5629586008178688, 11259172008099840, 22518341868716544, 9007336962655232, 18014673925310464, 2216338399232, 4432676798464, 11064376819712, 22137335185408, 44272556441600, 87995357200384, 35253226045952, 70506452091904, 567382630219776, 1134765260406784, 2832480465846272, 5667157807464448, 11333774449049600, 22526811443298304, 9024825867763712, 18049651735527936};
    // Moveable squares bitboard for bishop and rook taking into account the edge squares
    // Used for computing pin_bits in BitPosition class (setPinsBits and setChecksAndPinsBits)
    inline constexpr std::array<uint64_t, 64> rook_full_rays = {72340172838076926, 144680345676153597, 289360691352306939, 578721382704613623, 1157442765409226991, 2314885530818453727, 4629771061636907199, 9259542123273814143, 72340172838141441, 144680345676217602, 289360691352369924, 578721382704674568, 1157442765409283856, 2314885530818502432, 4629771061636939584, 9259542123273813888, 72340172854657281, 144680345692602882, 289360691368494084, 578721382720276488, 1157442765423841296, 2314885530830970912, 4629771061645230144, 9259542123273748608, 72340177082712321, 144680349887234562, 289360695496279044, 578721386714368008, 1157442769150545936, 2314885534022901792, 4629771063767613504, 9259542123257036928, 72341259464802561, 144681423712944642, 289361752209228804, 578722409201797128, 1157443723186933776, 2314886351157207072, 4629771607097753664, 9259542118978846848, 72618349279904001, 144956323094725122, 289632270724367364, 578984165983651848, 1157687956502220816, 2315095537539358752, 4629910699613634624, 9259541023762186368, 143553341945872641, 215330564830528002, 358885010599838724, 645993902138460168, 1220211685215703056, 2368647251370188832, 4665518383679160384, 9259260648297103488, 18302911464433844481, 18231136449196065282, 18087586418720506884, 17800486357769390088, 17226286235867156496, 16077885992062689312, 13781085504453754944, 9187484529235886208};
    inline constexpr std::array<uint64_t, 64> bishop_full_rays = {9241421688590303744, 36099303471056128, 141012904249856, 550848566272, 6480472064, 1108177604608, 283691315142656, 72624976668147712, 4620710844295151618, 9241421688590368773, 36099303487963146, 141017232965652, 1659000848424, 283693466779728, 72624976676520096, 145249953336262720, 2310355422147510788, 4620710844311799048, 9241421692918565393, 36100411639206946, 424704217196612, 72625527495610504, 145249955479592976, 290499906664153120, 1155177711057110024, 2310355426409252880, 4620711952330133792, 9241705379636978241, 108724279602332802, 145390965166737412, 290500455356698632, 580999811184992272, 577588851267340304, 1155178802063085600, 2310639079102947392, 4693335752243822976, 9386671504487645697, 326598935265674242, 581140276476643332, 1161999073681608712, 288793334762704928, 577868148797087808, 1227793891648880768, 2455587783297826816, 4911175566595588352, 9822351133174399489, 1197958188344280066, 2323857683139004420, 144117404414255168, 360293502378066048, 720587009051099136, 1441174018118909952, 2882348036221108224, 5764696068147249408, 11529391036782871041, 4611756524879479810, 567382630219904, 1416240237150208, 2833579985862656, 5667164249915392, 11334324221640704, 22667548931719168, 45053622886727936, 18049651735527937};
    // Bitboards of rays from square_1 to square_2, including square_1 and excluding square_2
    // Used setPinsBits and setChecksAndPinsBits.
    inline constexpr std::array<std::array<uint64_t, 64>, 64> precomputedBishopMovesTableOneBlocker {getBishopOneBlockerTable()};
    inline constexpr std::array<std::array<uint64_t, 64>, 64> precomputedRookMovesTableOneBlocker {getRookOneBlockerTable()};

    extern std::vector<std::map<uint64_t, uint64_t>> precomputedBishopMovesTable;
    extern std::vector<std::map<uint64_t, uint64_t>> precomputedRookMovesTable;
}
#endif