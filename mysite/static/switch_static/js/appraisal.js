

/*
Author:Eric
date:2013-04-12
purpose:获取当前时间
 */
function getDateTime(){
    var myDate= new Date();
    var time=  myDate.getFullYear()+'-'+(myDate.getMonth()+1)+'-'+myDate.getDate()+' '+myDate.getHours()+':'+myDate.getMinutes()+':'+myDate.getSeconds();
    return time;
}


/*
Author:Eric
date:2013-04-26
purpose:与数据库交互，插入打分评价信息
 */

function showAppraisal(food,enter_name,currpage){

    $.getJSON('/foodtrace/getallscore/?time=' + getDateTime(),{food_name:food,enterprise_name:enter_name,currentpage:currpage},function(data){
        if(data.result==''||data.result==null){
            var $score="<span id='scorerate'><a class='evaluate'> </a></span>";
            $('#scorerate').replaceWith($score);

            var $ev="<div id='evaul'><div class='comment'><div class='user'><a>暂无该类产品的评论</a></div></div></div>";
            $("#evaul").replaceWith($ev);
        }else{
        //好评率
         //   var $scor="<span id='scorerate'>好评率:&nbsp;<a class='evaluate'>"+((data.Good/data.TotalScore)*100).toFixed(1)+"%</a></span>";
         //   $('#scorerate').replaceWith($scor);
              var $scor="<span id='scorerate'>好评率:&nbsp;<a class='evaluate'>"+((data.Good/data.TotalScore)*100).toFixed(1)+"%</a></br>" +
                "好评数:&nbsp;<a class='evaluate'>"+data.Good+"</a></br>" +
                "差评数:&nbsp;<a class='evaluate'>"+(data.TotalScore-data.Good)+"</a>" +
                "</span>";
            $('#scorerate').replaceWith($scor);

        //评论
             var obj=data.result;
             var $evaul="<div id='evaul'></div>";
             $("#evaul").replaceWith($evaul);
             $.each(obj,function(index,element) {
                      var user = element['user'];
                      var time = element['time'];
                      var score = element['score'];
                      var appraisal = element['appraisal'];
                      showDetail(user, time, score, appraisal); //评论部分
             });

        //分页显示
             var total=data.totalpages;
             var pn=5;
             var str=pageBar(total,currpage,pn,food,enter_name);
             var $divpagenum="<div id='mapages'>&nbsp;&nbsp;&nbsp;&nbsp;" +str +"</div>";
             $("#mapages").replaceWith($divpagenum);
        }
    });
}

/*
Author:Eric
date:2013-04-26
purpose:显示打分评论信息
*/
function showDetail(user,time,score,appraisal){
       if (score==1){
            var tmp="<img src='/site_static/foodtrace_static/images/up.png' alt=' '>";
       }else{
             tmp="<img src='/site_static/foodtrace_static/images/down.png' alt=' '>";
       }
       var $divcomment="<div class='comment'>"+"<div class='user'><a>"+user+"</a>"+tmp+"</div>"+
                     "<div class='time'><a>"+time+"</a></div>"+
                     "<div class='message'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<p>"+appraisal+"</p></div>"+
                      "<div class='cutoffs'></div>"+"</div>";
       $("#evaul").append($divcomment);
}

/*
分页功能
*/
//总页数，当前页数，第一页和最后一页之间显示的页码数量

/*
分页功能
*/
//总页数，当前页数，第一页和最后一页之间显示的页码数量
function pageBar(tp,cp,pn,food,enter_name){
        var $str='';
        if(tp>1 && cp>1){
            var prev = cp-1;
            $str += "<a href=\"javascript:showAppraisal('"+food+"','"+enter_name+"','"+prev+"')\">"+"上一页 </a>&nbsp;&nbsp;&nbsp;&nbsp;";
        }else{
            $str += "<a href=\"javascript:void(0);\">"+"上一页</a>&nbsp;&nbsp;&nbsp;&nbsp;";
        }

        if(tp>1){
            //第一页
            if(cp==1){
                $str +="1&nbsp;&nbsp;&nbsp;&nbsp;" ;
            }else{
                $str +="<a href=\"javascript:showAppraisal('"+food+"','"+enter_name+"','"+1+"')\">"+"1 </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
            }

            if(tp>2){
                var pnh = Math.floor(pn/2);
                //循环开始页码
                var s = cp-pnh;
                if(s<=1){
                    s = 2;
                }
                //循环结束页码
                var e = cp+pnh;
                if(e>=tp){
                    e = tp-1;
                }

                if(s<=(1+pnh)){
                    if(tp>(pn+2)){
                        e = s+(pn-1);
                        if(e>=tp){
                            e = tp-1;
                        }
                    }else{
                        s = 2;
                    }
                }

                if(e>=(tp-pnh)){
                    if(tp>(pn+2)){
                        s = e-(pn-1);
                        if(s<=1){
                            s = 2;
                        }
                    }else{
                        e = tp-1;
                    }
                }

                if(e<s){
                    e = s;
                }

                //第一页后的多页跳转
                if(s>2){
                    var sp = cp-pn;
                    if(sp<1){
                        sp=1;
                    }
                     $str +="<a href=\"javascript:showAppraisal('"+food+"','"+enter_name+"','"+sp+"')\">"+" …  </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
                }

                for(var i=s;i<=e;i++){
                    if(i==cp){
                        $str +=" "+i+" &nbsp;&nbsp;&nbsp;&nbsp;" ;
                    }else{
                        $str +="<a href=\"javascript:showAppraisal('"+food+"','"+enter_name+"','"+i+"')\">"+" "+i+"  </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
                    }
                }

                //最后一页前的多页跳转
                if(e < (tp-1)){
                    var ep = cp+pn;
                    if(ep>tp){
                        ep=tp;
                    }
                    $str +="<a href=\"javascript:showAppraisal('"+food+"','"+enter_name+"','"+ep+"')\">"+" …  </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
                }
            }

            //最后一页
            if(cp==tp){
                $str +=" "+tp+" &nbsp;&nbsp;&nbsp;&nbsp;";
            }else{
                $str +="<a href=\"javascript:showAppraisal('"+food+"','"+enter_name+"','"+tp+"')\">"+" "+tp+" </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
            }

        }else{
              $str +="<a href=\"javascript:void(0);\">"+" "+1+" </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
        }

        if( tp>1 && cp<tp){
             var next = parseInt(cp) + 1 ;
             $str +="<a href=\"javascript:showAppraisal('"+food+"','"+enter_name+"','"+next+"')\">"+" 下一页  </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
        }else{
             $str +="<a href=\"javascript:void(0);\">"+" 下一页  </a>&nbsp;&nbsp;&nbsp;&nbsp;";
        }

        return $str;
    }


/*
Author:Eric
date:2013-04-26
purpose:与数据库交互，显示打分评价信息
 */
function showAppraisalByCode(itemCode,locaCode,currpage){

    $.getJSON('/foodtrace/getallscoreByCode/?time=' + getDateTime(),{itemCode:itemCode,locaCode:locaCode,currentpage:currpage},function(data){
        if(data.result==''||data.result==null){
            var $score="<span id='scorerate'><a class='evaluate'> </a></span>";
            $('#scorerate').replaceWith($score);

            var $ev="<div id='evaul'><div class='comment'><div class='user'><a>暂无该类产品的评论</a></div></div></div>";
            $("#evaul").replaceWith($ev);
        }else{
        //好评率
         //   var $scor="<span id='scorerate'>好评率:&nbsp;<a class='evaluate'>"+((data.Good/data.TotalScore)*100).toFixed(1)+"%</a></span>";
         //   $('#scorerate').replaceWith($scor);
              var $scor="<span id='scorerate'>好评率:&nbsp;<a class='evaluate'>"+((data.Good/data.TotalScore)*100).toFixed(1)+"%</a></br>" +
                "好评数:&nbsp;<a class='evaluate'>"+data.Good+"</a></br>" +
                "差评数:&nbsp;<a class='evaluate'>"+(data.TotalScore-data.Good)+"</a>" +
                "</span>";
            $('#scorerate').replaceWith($scor);

        //评论
             var obj=data.result;
             var $evaul="<div id='evaul'></div>";
             $("#evaul").replaceWith($evaul);
             $.each(obj,function(index,element) {
                      var user = element['user'];
                      var time = element['time'];
                      var score = element['score'];
                      var appraisal = element['appraisal'];
                      showDetail(user, time, score, appraisal); //评论部分
             });

        //分页显示
             var total=data.totalpages;
             var pn=5;
             var str=pageBarByCode(total,currpage,pn,itemCode,locaCode);
             var $divpagenum="<div id='mapages'>&nbsp;&nbsp;&nbsp;&nbsp;" +str +"</div>";
             $("#mapages").replaceWith($divpagenum);
        }
    });
}

/*
分页功能
*/
//总页数，当前页数，第一页和最后一页之间显示的页码数量
function pageBarByCode(tp,cp,pn,itemCode,locaCode){
        var $str='';
        if(tp>1 && cp>1){
            var prev = cp-1;
            $str += "<a href=\"javascript:showAppraisal('"+itemCode+"','"+locaCode+"','"+prev+"')\">"+"上一页 </a>&nbsp;&nbsp;&nbsp;&nbsp;";
        }else{
            $str += "<a href=\"javascript:void(0);\">"+"上一页</a>&nbsp;&nbsp;&nbsp;&nbsp;";
        }

        if(tp>1){
            //第一页
            if(cp==1){
                $str +="1&nbsp;&nbsp;&nbsp;&nbsp;" ;
            }else{
                $str +="<a href=\"javascript:showAppraisal('"+itemCode+"','"+locaCode+"','"+1+"')\">"+"1 </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
            }

            if(tp>2){
                var pnh = Math.floor(pn/2);
                //循环开始页码
                var s = cp-pnh;
                if(s<=1){
                    s = 2;
                }
                //循环结束页码
                var e = cp+pnh;
                if(e>=tp){
                    e = tp-1;
                }

                if(s<=(1+pnh)){
                    if(tp>(pn+2)){
                        e = s+(pn-1);
                        if(e>=tp){
                            e = tp-1;
                        }
                    }else{
                        s = 2;
                    }
                }

                if(e>=(tp-pnh)){
                    if(tp>(pn+2)){
                        s = e-(pn-1);
                        if(s<=1){
                            s = 2;
                        }
                    }else{
                        e = tp-1;
                    }
                }

                if(e<s){
                    e = s;
                }

                //第一页后的多页跳转
                if(s>2){
                    var sp = cp-pn;
                    if(sp<1){
                        sp=1;
                    }
                     $str +="<a href=\"javascript:showAppraisal('"+itemCode+"','"+locaCode+"','"+sp+"')\">"+" …  </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
                }

                for(var i=s;i<=e;i++){
                    if(i==cp){
                        $str +=" "+i+" &nbsp;&nbsp;&nbsp;&nbsp;" ;
                    }else{
                        $str +="<a href=\"javascript:showAppraisal('"+itemCode+"','"+locaCode+"','"+i+"')\">"+" "+i+"  </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
                    }
                }

                //最后一页前的多页跳转
                if(e < (tp-1)){
                    var ep = cp+pn;
                    if(ep>tp){
                        ep=tp;
                    }
                    $str +="<a href=\"javascript:showAppraisal('"+itemCode+"','"+locaCode+"','"+ep+"')\">"+" …  </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
                }
            }

            //最后一页
            if(cp==tp){
                $str +=" "+tp+" &nbsp;&nbsp;&nbsp;&nbsp;";
            }else{
                $str +="<a href=\"javascript:showAppraisal('"+itemCode+"','"+locaCode+"','"+tp+"')\">"+" "+tp+" </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
            }

        }else{
              $str +="<a href=\"javascript:void(0);\">"+" "+1+" </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
        }

        if( tp>1 && cp<tp){
             var next = parseInt(cp) + 1 ;
             $str +="<a href=\"javascript:showAppraisal('"+itemCode+"','"+locaCode+"','"+next+"')\">"+" 下一页  </a>&nbsp;&nbsp;&nbsp;&nbsp;" ;
        }else{
             $str +="<a href=\"javascript:void(0);\">"+" 下一页  </a>&nbsp;&nbsp;&nbsp;&nbsp;";
        }

        return $str;
    }