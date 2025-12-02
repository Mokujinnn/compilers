target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [18 x i8] c"gcd(12, 15) = %d\0A\00", align 1

define i32 @gcd(i32 %a, i32 %b) {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %b, ptr %b.addr, align 4
  br label %while.cond

while.cond:
  %b1 = load i32, ptr %b.addr, align 4
  %cmp = icmp eq i32 %b1, 0
  br i1 %cmp, label %end, label %while.body

while.body:
  %a1 = load i32, ptr %a.addr, align 4
  %b2 = load i32, ptr %b.addr, align 4
  %rem = srem i32 %a1, %b2
  
  store i32 %b2, ptr %a.addr, align 4
  store i32 %rem, ptr %b.addr, align 4
  
  br label %while.cond

end:
  %a.result = load i32, ptr %a.addr, align 4
  ret i32 %a.result
}

define dso_local i32 @main() {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %2 = call i32 @gcd(i32 noundef 12, i32 noundef 15)
  %3 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %2)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...)
