; ModuleID = './test/naive2.c'
source_filename = "./test/naive2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct._object = type { i64, ptr }

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local ptr @ret_same(ptr noundef %a) #0 {
entry:
  %a.addr = alloca ptr, align 8
  store ptr %a, ptr %a.addr, align 8
  %0 = load ptr, ptr %a.addr, align 8
  call void @Py_INCREF(ptr noundef %0)
  %1 = load ptr, ptr %a.addr, align 8
  ret ptr %1
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @Py_INCREF(ptr noundef %op) #0 {
entry:
  %op.addr = alloca ptr, align 8
  store ptr %op, ptr %op.addr, align 8
  %0 = load ptr, ptr %op.addr, align 8
  %ob_refcnt = getelementptr inbounds %struct._object, ptr %0, i32 0, i32 0
  %1 = load i64, ptr %ob_refcnt, align 8
  %inc = add nsw i64 %1, 1
  store i64 %inc, ptr %ob_refcnt, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i64 @sum_list(ptr noundef %list) #0 {
entry:
  %retval = alloca i64, align 8
  %list.addr = alloca ptr, align 8
  %i = alloca i32, align 4
  %n = alloca i32, align 4
  %total = alloca i64, align 8
  %item = alloca ptr, align 8
  store ptr %list, ptr %list.addr, align 8
  store i64 0, ptr %total, align 8
  %0 = load ptr, ptr %list.addr, align 8
  %call = call i64 @PyList_Size(ptr noundef %0)
  %conv = trunc i64 %call to i32
  store i32 %conv, ptr %n, align 4
  %1 = load i32, ptr %n, align 4
  %cmp = icmp slt i32 %1, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i64 -1, ptr %retval, align 8
  br label %return

if.end:                                           ; preds = %entry
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %2 = load i32, ptr %i, align 4
  %3 = load i32, ptr %n, align 4
  %cmp2 = icmp slt i32 %2, %3
  br i1 %cmp2, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %4 = load ptr, ptr %list.addr, align 8
  %5 = load i32, ptr %i, align 4
  %conv4 = sext i32 %5 to i64
  %call5 = call ptr @PyList_GetItem(ptr noundef %4, i64 noundef %conv4)
  store ptr %call5, ptr %item, align 8
  %6 = load ptr, ptr %item, align 8
  %call6 = call i64 @PyLong_AsLong(ptr noundef %6)
  %7 = load i64, ptr %total, align 8
  %add = add nsw i64 %7, %call6
  store i64 %add, ptr %total, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %8 = load i32, ptr %i, align 4
  %inc = add nsw i32 %8, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  %9 = load ptr, ptr %list.addr, align 8
  %call7 = call ptr @ret_same(ptr noundef %9)
  store ptr %call7, ptr %list.addr, align 8
  %10 = load ptr, ptr %item, align 8
  call void @Py_DECREF(ptr noundef %10)
  %11 = load i64, ptr %total, align 8
  store i64 %11, ptr %retval, align 8
  br label %return

return:                                           ; preds = %for.end, %if.then
  %12 = load i64, ptr %retval, align 8
  ret i64 %12
}

declare i64 @PyList_Size(ptr noundef) #1

declare ptr @PyList_GetItem(ptr noundef, i64 noundef) #1

declare i64 @PyLong_AsLong(ptr noundef) #1

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define internal void @Py_DECREF(ptr noundef %op) #0 {
entry:
  %op.addr = alloca ptr, align 8
  store ptr %op, ptr %op.addr, align 8
  %0 = load ptr, ptr %op.addr, align 8
  %ob_refcnt = getelementptr inbounds %struct._object, ptr %0, i32 0, i32 0
  %1 = load i64, ptr %ob_refcnt, align 8
  %dec = add nsw i64 %1, -1
  store i64 %dec, ptr %ob_refcnt, align 8
  %cmp = icmp eq i64 %dec, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %2 = load ptr, ptr %op.addr, align 8
  call void @_Py_Dealloc(ptr noundef %2)
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  ret void
}

declare void @_Py_Dealloc(ptr noundef) #1

attributes #0 = { noinline nounwind optnone sspstrong uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 15.0.7"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
