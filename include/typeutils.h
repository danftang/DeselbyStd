//
// Created by daniel on 27/09/23.
//

#ifndef POLYMORPHICCOLLECTION_TYPEUTILS_H
#define POLYMORPHICCOLLECTION_TYPEUTILS_H

#include <cstdlib>
#include <utility>
#include <variant>

namespace deselby {
    /** true if T is a type of the from TEMPLATE<class...> for some set of classes */
    template<class T, template<class...> class TEMPLATE>
    struct is_class_template_of {
        static constexpr bool value = false;
    };

    template<template<class...> class TEMPLATEDCLASS, class...TEMPLATES>
    struct is_class_template_of<TEMPLATEDCLASS<TEMPLATES...>, TEMPLATEDCLASS> {
        static constexpr bool value = true;
    };

    template<template<class...> class TEMPLATEDCLASS, class...TEMPLATES>
    struct is_class_template_of<const TEMPLATEDCLASS<TEMPLATES...>, TEMPLATEDCLASS> {
        static constexpr bool value = true;
    };

    template<class T, template<class...> class TEMPLATE>
    static constexpr bool is_class_template_of_v = is_class_template_of<T, TEMPLATE>::value;

    template<class T, template<class...> class TEMPLATE>
    concept IsClassTemplateOf = is_class_template_of_v<T, TEMPLATE>;


/** Expresses a number as a sequence of digits in a given base
 *
 * @tparam N        the number to express
 * @tparam digits   how many digits to expand
 * @tparam base     the base to use in expansion
 * @tparam suffix   an optional suffix which will be added to the end of the expansion
 */
    template<size_t N, size_t digits, size_t base, size_t...suffix>
    struct base_n_expansion : base_n_expansion<N / base, digits - 1, base, N % base, suffix...> {
    };

    template<size_t N, size_t base, size_t... suffix>
    struct base_n_expansion<N, 1, base, suffix...> {
        using type = typename std::index_sequence<N % base, suffix...>;
    };

    template<size_t N, size_t digits, size_t base>
    using base_n_expansion_t = typename base_n_expansion<N, digits, base>::type;

/** True if T is a member of OTHERS */
    template<class T, class... OTHERS>
    concept IsIn = (std::same_as<std::remove_cvref_t<T>, std::remove_cvref_t<OTHERS>> || ...);

/** Alldifferent<class...TS> is true if all classes TS... are distinct and TS is not empty */
    template<class T, class...OTHERS> requires(!IsIn<T, OTHERS...>)
    struct AllDifferentHelper : AllDifferentHelper<OTHERS...> {
    };

    template<class T>
    struct AllDifferentHelper<T> {
    };

    template<class... CLASSES>
    concept AllDifferentAndNotEmpty = requires() { AllDifferentHelper<CLASSES...>(); };

/** Turns any reference into a reference_wrapper, otherwise returns the original type */
    template<class T>
    using wrap_if_reference = std::conditional_t<
            std::is_reference_v<T>,
            std::reference_wrapper<std::remove_reference_t<T>>,
            T>;


/** Turn a tuple type into a variant of reference_wrappers to the elements of the tuple */
    template<class TUPLE>
    struct reference_variant_type;
    template<class... TELEMENTS>
    struct reference_variant_type<std::tuple<TELEMENTS...>> {
        using type = std::variant<std::reference_wrapper<std::remove_reference_t<TELEMENTS>>...>;
    };
    template<class... TELEMENTS>
    struct reference_variant_type<const std::tuple<TELEMENTS...>> {
        using type = std::variant<std::reference_wrapper<const std::remove_reference_t<TELEMENTS>>...>;
    };
    template<class TUPLE>
    using reference_variant_type_t = typename reference_variant_type<TUPLE>::type;


/** Turn a tuple type into a variant type with the same elements, but wrapping any referenes in reference_wrapper */
    template<class TUPLE>
    struct variant_type;
    template<class... TELEMENTS>
    struct variant_type<std::tuple<TELEMENTS...>> {
        using type = std::variant<wrap_if_reference<TELEMENTS>...>;
    };
    template<class... TELEMENTS>
    struct variant_type<const std::tuple<TELEMENTS...>> {
        using type = std::variant<wrap_if_reference<const TELEMENTS>...>;
    };
    template<class TUPLE>
    using variant_type_t = typename variant_type<TUPLE>::type;

/** Provides a compile-time x^y for integer x and unsigned integer y */
    template<std::integral T>
    consteval T powi(T base, uint index) {
        if (index == 2) return base * base;
        if (index == 0) return 1;
        return (index & 1) ? powi(powi(base, index / 2), 2) * base : powi(powi(base, index / 2), 2);
    }

    // True if T is an STL container
    template<class T>
    struct is_stl_container : public std::false_type {
    };
    template<class T>
    struct is_stl_container<std::vector<T>> : public std::true_type {
    };
    template<class T>
    struct is_stl_container<std::list<T>> : public std::true_type {
    };
    template<class T>
    struct is_stl_container<std::forward_list<T>> : public std::true_type {
    };
    template<class T>
    struct is_stl_container<std::deque<T>> : public std::true_type {
    };
    template<class T>
    struct is_stl_container<std::set<T>> : public std::true_type {
    };
    template<class T>
    struct is_stl_container<std::unordered_set<T>> : public std::true_type {
    };
    template<class T>
    struct is_stl_container<std::valarray<T>> : public std::true_type {
    };
    template<class T, int N>
    struct is_stl_container<std::array<T, N>> : public std::true_type {
    };
    template<class KEY, class VAL>
    struct is_stl_container<std::map<KEY, VAL>> : public std::true_type {
    };
    template<class KEY, class VAL>
    struct is_stl_container<std::multimap<KEY, VAL>> : public std::true_type {
    };
    template<class T> inline constexpr bool is_stl_container_v = is_stl_container<T>::value;


    // True if T is an associative STL container
    template<class T>
    struct is_stl_associative_container : public std::false_type {
    };
    template<class KEY, class VAL>
    struct is_stl_associative_container<std::map<KEY, VAL>> : public std::true_type {
    };
    template<class KEY, class VAL>
    struct is_stl_associative_container<std::multimap<KEY, VAL>> : public std::true_type {
    };
    template<class T> inline constexpr bool is_stl_associative_container_v = is_stl_associative_container<T>::value;


    // True if T is any kind of pair
//    template<class T>
//    struct is_pair: public is_class {};
//    template<class A, class B>
//    struct is_pair<std::pair<A,B>>: public std::true_type { };
    template<class T> inline constexpr bool is_pair_v = is_class_template_of_v<T,std::pair>;
    template<class T> inline constexpr bool is_optional_v = is_class_template_of_v<T,std::optional>;
    template<class T> inline constexpr bool is_tuple_v = is_class_template_of_v<T,std::tuple>;

}

#endif //POLYMORPHICCOLLECTION_TYPEUTILS_H