/*
autho:Eric
date:2013.5.23
purpose: form validation for register
 */

  function check(){
             var name=document.getElementById("username");
             var nickname=document.getElementById("nickname");
             var pwd1=document.getElementById("password1");
             var pwd2=document.getElementById("password2");
             var tel=document.getElementById("phone");
             var re= /^(1[0-9]{10})$/;
             var patt=new RegExp(re);

             if(name.value.length<5){
                alert('用户名不能小于5位');
                document.getElementById("username").value='';
                document.getElementById("username").focus();
                return false;
             }
             if(nickname.value.trim()=='' || nickname.value==null){
                 alert('昵称不能为空');
                 document.getElementById("nickname").focus();
                 return false;
             }
             if(pwd1.value.length<6){
                 alert('密码长度不能小于6');
                 document.getElementById("password1").value='';
                 document.getElementById("password2").value='';
                 document.getElementById("password1").focus();
                 return false;
             }
             if(pwd1.value!=pwd2.value){
                 alert('密码不一致');
                 document.getElementById("password1").value='';
                 document.getElementById("password2").value='';
                 document.getElementById("password1").focus();
                 return false;
              }
             if(!patt.test(tel.value)){
                 alert('请输入正确的手机号');
                 document.getElementById("phone").value='';
                 document.getElementById("phone").focus();
                 return false;
             }
             return true;
  }

/*
autho:Eric
date:2013.5.23
purpose: form validation for user password change
 */

  function checkUserPwd(){
             var pwd1=document.getElementById("newpwd1");
             var pwd2=document.getElementById("newpwd2");

             if(pwd1.value.length<6){
                 alert('密码长度不能小于6');
                 document.getElementById("newpwd1").value='';
                 document.getElementById("newpwd2").value='';
                 document.getElementById("newpwd1").focus();
                 return false;
             }
             if(pwd1.value!=pwd2.value){
                 alert('密码不一致');
                 document.getElementById("newpwd1").value='';
                 document.getElementById("newpwd2").value='';
                 document.getElementById("newpwd1").focus();
                 return false;
              }
      
             return true;
  }
