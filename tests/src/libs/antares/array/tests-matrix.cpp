#define BOOST_TEST_MODULE test-lib-antares-matrix tests

#define WIN32_LEAN_AND_MEAN

#include <boost/test/included/unit_test.hpp>

#include<tests-matrix.h>

namespace utf = boost::unit_test;


// ================================
// ===  Matrix<double, double>  ===
// ================================
BOOST_AUTO_TEST_SUITE(coeffs_are_double__save_into_double)
BOOST_AUTO_TEST_CASE(matrix_only_0s_and_no_print_dim___result_is_empty)
{
	Matrix_enhanced<double, double> mtx;
	mtx.reset(2, 2, true);
	mtx.saveToCSVFile("path/to/an/output/file");
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "");
}

BOOST_AUTO_TEST_CASE(matrix_only_0s_print_dim__get_only_a_title_and_the_0s)
{
	Matrix_enhanced<double, double> mtx;
	mtx.reset(2, 2, true);
	mtx.saveToCSVFile("path/to/an/output/file", 0, true);
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "size:2x2\n0\t0\n0\t0\n");
}

BOOST_AUTO_TEST_CASE(coeffs_have_int_values__no_zeros_after_decimal_point)
{
	Matrix_enhanced<double, double> mtx(2, 2, {1, 0, 0, 2});
	mtx.saveToCSVFile("path/to/an/output/file");
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "1\t0\n0\t2\n");
}

BOOST_AUTO_TEST_CASE(precision_is_3__get_3_nbs_after_decimal_point)
{
	Matrix_enhanced<double, double> mtx(2, 2, {1.5, 2.44444, 3.66666, 0});
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "1.500\t2.444\n3.667\t0\n");
}

BOOST_AUTO_TEST_CASE(precision_has_no_effect_on_integer_values)
{
	// Any whole number is printed without decimal point
	Matrix_enhanced<double, double> mtx(2, 2, {1, 5, 0, 3});
	mtx.saveToCSVFile("path/to/an/output/file", 4, false);
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "1\t5\n0\t3\n");
}

BOOST_AUTO_TEST_CASE(add_predicate__each_coeff_is_one_incremented)
{
	Matrix_enhanced<double, double> mtx(2, 2, {1, 0, 0, 2});
	TSNumbersPredicate predicate;
	mtx.saveToCSVFile("path/to/an/output/file", 2, false, predicate);
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "2\t1\n1\t3\n");
}

BOOST_AUTO_TEST_CASE(TS_nb_predicate_suits_int_matrices__It_converts_double_into_unsigned_ints)
{
	// Negative numbers are turned into strange positive numbers because the TS number
	// predicate converted them into unsigned int
	Matrix_enhanced<double, double> mtx(2, 2, {1.5, -3.552, 0.66, 2.3});
	TSNumbersPredicate predicate;
	mtx.saveToCSVFile("path/to/an/output/file", 2, false, predicate);
	string res = mtx.data.to<string>();
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "2\t4294967294\n1\t3\n");
}
BOOST_AUTO_TEST_SUITE_END()



// =============================
// ===  Matrix<int, double>  ===
// =============================
BOOST_AUTO_TEST_SUITE(coeffs_are_int__save_into_double)
BOOST_AUTO_TEST_CASE(matrix_only_0s_and_no_print_dim___result_is_empty)
{
	Matrix_enhanced<int, double> mtx(2, 2);
	mtx.reset(2, 2, true);
	mtx.saveToCSVFile("path/to/an/output/file");
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "");
}

BOOST_AUTO_TEST_CASE(any_whole_number_is_printed_without_decimal_point)
{
	Matrix_enhanced<int, double> mtx(2, 2, {10, 500, 0, 3});
	mtx.saveToCSVFile("path/to/an/output/file");
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "10\t500\n0\t3\n");
}
BOOST_AUTO_TEST_SUITE_END()



// ==========================
// ===  Matrix<int, int>  ===
// ==========================
BOOST_AUTO_TEST_SUITE(coeffs_are_int__save_into_int)
BOOST_AUTO_TEST_CASE(matrix_only_0s_and_no_print_dim___result_is_empty)
{
	Matrix_enhanced<int, int> mtx(2, 2);
	mtx.reset(2, 2, true);
	mtx.saveToCSVFile("path/to/an/output/file");
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "");
}

BOOST_AUTO_TEST_CASE(first_matrix__int_to_int)
{
	Matrix_enhanced<int, int> mtx(2, 2, {1000, -5000, 0, 3000});
	mtx.saveToCSVFile("path/to/an/output/file");
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "1000\t-5000\n0\t3000\n");
}
BOOST_AUTO_TEST_SUITE_END()


// =============================
// ===  Matrix<double, int>  ===
// =============================
BOOST_AUTO_TEST_SUITE(coeffs_are_double__save_into_int)
BOOST_AUTO_TEST_CASE(precision_is_3__get_coeffs_floor_integers)
{
	Matrix_enhanced<double, int> mtx(2, 2, {1.99, 2.44, -3.999, -1.51});
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_REQUIRE_EQUAL(mtx.data.to<string>(), "1\t2\n-3\t-1\n");
}
BOOST_AUTO_TEST_SUITE_END()


// ===================
// ===  About JIT  ===
// ===================

BOOST_AUTO_TEST_SUITE(jit)

BOOST_AUTO_TEST_CASE(JIT_enabler_built__jit_also_built)
{
	{
		global_JIT_manager global_JIT_to(true);
		BOOST_CHECK(JIT::enabled);
	}
	BOOST_CHECK(not JIT::enabled);
}

BOOST_AUTO_TEST_CASE(global_JIT_off___matrix_built_with_any_constructor_or_reset____jit_not_built)
{
	Matrix_load_bypass<double, double> mtx_1, mtx_2(2, 2);
	BOOST_CHECK(not mtx_1.jit);
	BOOST_CHECK(not mtx_2.jit);
	mtx_1.reset(2, 2, true);
	BOOST_CHECK(not mtx_1.jit);
}

BOOST_AUTO_TEST_CASE(global_JIT_on___matrix_built_with_any_constructor____jit_not_built)
{
	global_JIT_manager global_JIT_to(true);
	Matrix_load_bypass<double, double> mtx_1, mtx_2(2, 2);
	BOOST_CHECK(not mtx_1.jit);
	BOOST_CHECK(not mtx_2.jit);
	mtx_1.reset(2, 2, true);
	BOOST_CHECK(mtx_1.jit);
}

BOOST_AUTO_TEST_CASE(global_JIT_on___matrix_built_with_reset____jit_built)
{
	global_JIT_manager global_JIT_to(true);
	Matrix_load_bypass<double, double> mtx;
	mtx.reset(2, 2, true);
	BOOST_CHECK(mtx.jit);
}

BOOST_AUTO_TEST_CASE(global_JIT_ON__reset__save____no_load_done)
{
	global_JIT_manager global_JIT_to(true);
	Matrix_load_bypass<double, double> mtx;
	mtx.reset(2, 2, true);
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_CHECK(not mtx.loadFromCSVFile_called);
	BOOST_CHECK(not mtx.jit->loadDataIfNotAlreadyDone);
	BOOST_CHECK(not mtx.jit->alreadyLoaded);
}

BOOST_AUTO_TEST_CASE(global_JIT_ON__reset__save__force_loadData_to_ON____no_load_done)
{
	global_JIT_manager global_JIT_to(true);
	Matrix_load_bypass<double, double> mtx;
	mtx.reset(2, 2, true);
	mtx.jit->loadDataIfNotAlreadyDone = true;
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_CHECK(not mtx.loadFromCSVFile_called);
	BOOST_CHECK(not mtx.jit->loadDataIfNotAlreadyDone);
	BOOST_CHECK(not mtx.jit->alreadyLoaded);
}

BOOST_AUTO_TEST_CASE(global_JIT_ON__reset__save__force_loadData_to_ON__alreadyLoaded_to_OFF____no_load_done)
{
	global_JIT_manager global_JIT_to(true);
	Matrix_load_bypass<double, double> mtx;
	mtx.reset(2, 2, true);
	mtx.jit->loadDataIfNotAlreadyDone = true;
	mtx.jit->alreadyLoaded = false;
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_CHECK(mtx.loadFromCSVFile_called);
	BOOST_CHECK(not mtx.jit->loadDataIfNotAlreadyDone);
}

BOOST_AUTO_TEST_CASE(matrix_not_empty_cleared_BEFORE_it_can_be_saved)
{
	global_JIT_manager global_JIT_to(true);
	Matrix_load_bypass<double, double> mtx(2, 2, { 1.99, 2.44, -3.999, -1.51 });
	mtx.jit = JIT::Reset(mtx.jit);
	mtx.jit->modified = false;
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_CHECK(not mtx.loadFromCSVFile_called);
	BOOST_CHECK(mtx.empty());
}

BOOST_AUTO_TEST_CASE(global_JIT_ON___matrix_saved_and_cleared)
{
	global_JIT_manager global_JIT_to(true);
	Matrix_load_bypass<double, double> mtx(2, 2, { 1.99, 2.44, -3.999, -1.51 });
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_CHECK(mtx.empty());
}

BOOST_AUTO_TEST_CASE(global_JIT_OFF_jit_off___matrix_never_cleared)
{
	global_JIT_manager global_JIT_to(false);
	Matrix_load_bypass<double, double> mtx(2, 2, { 1.99, 2.44, -3.999, -1.51 });
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_CHECK(not mtx.empty());
}

BOOST_AUTO_TEST_CASE(global_JIT_ON_jit_off___matrix_cleared)
{
	global_JIT_manager global_JIT_to(true);
	Matrix_load_bypass<double, double> mtx(2, 2, { 1.99, 2.44, -3.999, -1.51 });
	delete mtx.jit; // Turning jit off 
	mtx.jit = nullptr;
	mtx.saveToCSVFile("path/to/an/output/file", 3, false);
	BOOST_CHECK(mtx.empty());
}

BOOST_AUTO_TEST_SUITE_END()