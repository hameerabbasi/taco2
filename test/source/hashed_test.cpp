#include <doctest/doctest.h>
#include <xsparse/levels/hashed.hpp>
#include <xsparse/levels/dense.hpp>
#include <xsparse/levels/singleton.hpp>

#include <vector>
#include <unordered_map>

TEST_CASE("Dense-Hashed")
{
    constexpr uintptr_t SIZE0 = 3;
    constexpr uint8_t ZERO = 0;
    constexpr uintptr_t SIZE1 = 3;
    constexpr uintptr_t SIZE2 = 7;

    std::unordered_map<uintptr_t, uintptr_t> const umap1{ { 5, 2 }, { 6, 1 }, { 4, 0 } };
    std::unordered_map<uintptr_t, uintptr_t> const umap2{ { 2, 5 } };
    std::unordered_map<uintptr_t, uintptr_t> const umap3{ { 3, 3 }, { 1, 4 }, { 0, 6 } };

    std::vector<std::unordered_map<uintptr_t, uintptr_t>> const crd{ umap1, umap2, umap3 };

    xsparse::levels::dense<std::tuple<>, uintptr_t, uintptr_t> d{ SIZE0 };

    xsparse::levels::hashed<std::tuple<decltype(d)>,
                            uintptr_t,
                            uintptr_t,
                            std::vector<std::unordered_map<uintptr_t, uintptr_t>>>
        h{ SIZE1, SIZE2, crd };

    uintptr_t l1 = 0;
    for (auto const [i1, p1] : d.iter_helper(std::make_tuple(), ZERO))
    {
        CHECK(l1 == i1);
        CHECK(l1 == p1);
        uintptr_t l2 = 0;
        for (auto const [i2, p2] : h.iter_helper(p1))
        {
            CHECK(crd[l1].at(i2) == p2);
            ++l2;
        }
        CHECK(l2 == crd[l1].size());
        ++l1;
    }
    CHECK(l1 == SIZE0);
}

TEST_CASE("Hashed-Hashed")
{
    constexpr uintptr_t SIZE0 = 1;
    constexpr uintptr_t SIZE1 = 3;
    constexpr uint8_t ZERO = 0;
    constexpr uintptr_t SIZE2 = 3;
    constexpr uintptr_t SIZE3 = 7;

    std::unordered_map<uintptr_t, uintptr_t> const umap1{ { 0, 1 }, { 2, 0 }, { 1, 2 } };

    std::unordered_map<uintptr_t, uintptr_t> const umap2{ { 5, 2 }, { 6, 1 }, { 4, 0 } };
    std::unordered_map<uintptr_t, uintptr_t> const umap3{ { 2, 5 } };
    std::unordered_map<uintptr_t, uintptr_t> const umap4{ { 3, 3 }, { 1, 4 }, { 0, 6 } };

    std::vector<std::unordered_map<uintptr_t, uintptr_t>> const crd0{ umap1 };

    std::vector<std::unordered_map<uintptr_t, uintptr_t>> const crd1{ umap2, umap3, umap4 };

    xsparse::levels::hashed<std::tuple<>,
                            uintptr_t,
                            uintptr_t,
                            std::vector<std::unordered_map<uintptr_t, uintptr_t>>>
        h{ SIZE0, SIZE1, crd0 };

    xsparse::levels::hashed<std::tuple<decltype(h)>,
                            uintptr_t,
                            uintptr_t,
                            std::vector<std::unordered_map<uintptr_t, uintptr_t>>>
        h1{ SIZE2, SIZE3, crd1 };

    uintptr_t l1 = 0;
    for (auto const [i1, p1] : h.iter_helper(ZERO))
    {
        CHECK(crd0[ZERO].at(i1) == p1);
        uintptr_t l2 = 0;
        for (auto const [i2, p2] : h1.iter_helper(p1))
        {
            CHECK(crd1[p1].at(i2) == p2);
            ++l2;
        }
        CHECK(l2 == crd1[p1].size());
        ++l1;
    }
    CHECK(l1 == crd0[ZERO].size());
}

TEST_CASE("Hashed-Singleton")
{
    constexpr uintptr_t SIZE0 = 1;
    constexpr uintptr_t SIZE1 = 3;
    constexpr uintptr_t SIZE2 = 3;
    constexpr uint8_t ZERO = 0;

    std::unordered_map<uintptr_t, uintptr_t> const umap1{ { 0, 1 }, { 2, 0 }, { 1, 2 } };

    std::vector<std::unordered_map<uintptr_t, uintptr_t>> const crd0{ umap1 };

    std::vector<uintptr_t> const crd1{ 0, 3, 4 };

    xsparse::levels::hashed<std::tuple<>,
                            uintptr_t,
                            uintptr_t,
                            std::vector<std::unordered_map<uintptr_t, uintptr_t>>>
        h{ SIZE0, SIZE1, crd0 };

    xsparse::levels::
        singleton<std::tuple<decltype(h)>, uintptr_t, uintptr_t, std::vector<uintptr_t>>
            s{ SIZE2, crd1 };

    uintptr_t l1 = 0;
    for (auto const [i1, p1] : h.iter_helper(ZERO))
    {
        CHECK(crd0[ZERO].at(i1) == p1);
        uintptr_t l2 = p1;
        for (auto const [i2, p2] : s.iter_helper(std::make_tuple(i1), p1))
        {
            CHECK(l2 == p2);
            CHECK(crd1[l2] == i2);
            ++l2;
        }
        CHECK(l2 == p1 + 1);
        ++l1;
    }
    CHECK(l1 == crd0[ZERO].size());
}

TEST_CASE("Hashed-Singleton-Dense")
{
    constexpr uintptr_t SIZE0 = 1;
    constexpr uintptr_t SIZE1 = 5;
    constexpr uintptr_t SIZE2 = 1;
    constexpr uintptr_t SIZE3 = 2;
    constexpr uintptr_t ZERO = 0;

    std::unordered_map<uintptr_t, uintptr_t> umap1{
        { 1, 0 }, { 0, 2 }, { 2, 1 }, { 3, 4 }, { 4, 3 }
    };

    std::vector<std::unordered_map<uintptr_t, uintptr_t>> const crd0{ umap1 };
    std::vector<uintptr_t> const crd1 = { 0, 2, 0, 2, 3 };

    xsparse::levels::hashed<std::tuple<>,
                            uintptr_t,
                            uintptr_t,
                            std::vector<std::unordered_map<uintptr_t, uintptr_t>>>
        h{ SIZE0, SIZE1, crd0 };
    xsparse::levels::
        singleton<std::tuple<decltype(h)>, uintptr_t, uintptr_t, std::vector<uintptr_t>>
            s{ SIZE2, crd1 };
    xsparse::levels::dense<std::tuple<decltype(s)>, uintptr_t, uintptr_t> d{ SIZE3 };

    uintptr_t l1 = 0;
    for (auto const [i1, p1] : h.iter_helper(ZERO))
    {
        CHECK(crd0[ZERO].at(i1) == p1);
        uintptr_t l2 = p1;
        for (auto const [i2, p2] : s.iter_helper(std::make_tuple(i1), p1))
        {
            CHECK(l2 == p2);
            CHECK(crd1[l2] == i2);
            uintptr_t l3 = 0;
            for (auto const [i3, p3] : d.iter_helper(std::make_tuple(i2), p2))
            {
                CHECK(l3 == i3);
                CHECK(l3 + SIZE3 * l2 == p3);
                ++l3;
            }
            CHECK(l3 == SIZE3);
            ++l2;
        }
        CHECK(l2 == p1 + 1);
        ++l1;
    }
    CHECK(l1 == crd0[ZERO].size());
}
