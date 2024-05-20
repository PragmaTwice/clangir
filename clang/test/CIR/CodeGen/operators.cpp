// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -fclangir -emit-cir %s -o %t.cir
// RUN: FileCheck --input-file=%t.cir %s

class __attribute__((__visibility__("default"))) exception_ptr
{
    void* __ptr_;
public:
    explicit operator bool() const noexcept {return __ptr_ != nullptr;}
    // CHECK: @_ZNK13exception_ptrcvbEv
    // CHECK: %0 = cir.alloca !cir.ptr<!ty_22exception_ptr22>, !cir.ptr<!cir.ptr<!ty_22exception_ptr22>>, ["this", init] {alignment = 8 : i64}
    // CHECK: %1 = cir.alloca !cir.bool, !cir.ptr<!cir.bool>, ["__retval"] {alignment = 1 : i64}
    // CHECK: cir.store %arg0, %0 : !cir.ptr<!ty_22exception_ptr22>, !cir.ptr<!cir.ptr<!ty_22exception_ptr22>>
    // CHECK: %2 = cir.load %0 : !cir.ptr<!cir.ptr<!ty_22exception_ptr22>>, !cir.ptr<!ty_22exception_ptr22>
    // CHECK: %3 = cir.get_member %2[0] {name = "__ptr_"} : !cir.ptr<!ty_22exception_ptr22> -> !cir.ptr<!cir.ptr<!void>>
    // CHECK: %4 = cir.load %3 : !cir.ptr<!cir.ptr<!void>>, !cir.ptr<!void>
    // CHECK: %5 = cir.const #cir.ptr<null> : !cir.ptr<!void>
    // CHECK: %6 = cir.cmp(ne, %4, %5) : !cir.ptr<!void>, !cir.bool
    // CHECK: cir.store %6, %1 : !cir.bool, !cir.ptr<!cir.bool>
    // CHECK: %7 = cir.load %1 : !cir.ptr<!cir.bool>, !cir.bool
    // CHECK: cir.return %7 : !cir.bool
};

bool f(const exception_ptr &ptr) { return (bool)ptr; }
// CHECK: @_Z1fRK13exception_ptr
// CHECK: %[[ptr:.*]] = cir.load %0 : !cir.ptr<!cir.ptr<!ty_22exception_ptr22>>, !cir.ptr<!ty_22exception_ptr22>
// CHECK: {{.*}} = cir.call @_ZNK13exception_ptrcvbEv(%[[ptr]]) : (!cir.ptr<!ty_22exception_ptr22>) -> !cir.bool
