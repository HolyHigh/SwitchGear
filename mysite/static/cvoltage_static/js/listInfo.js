/**
 * Created with PyCharm.
 * User: joe
 * Date: 14-6-25
 * Time: 下午5:11
 * To change this template use File | Settings | File Templates.
 */
function init(){
    list_Info();
}

function list_Info(){
    $.getJSON("/showInfo/showStation/",
        function(data){
            var stationdata= data['incident'];
            $.each(stationdata,function(entryIndex,entry) {
                var sid=entry['sid']
                var sname= entry['sname'];
                citem=entry['citem'];
                listStation(sid,sname,entryIndex);
            });
        })

    function listStation(sid,sname,entryIndex){
        var contentString;
        contentString=
            "<ul class='other' id='" +entryIndex+"'>"+
                "<a   href='javascript:void(0)'>" +
                sname +"</a>" + "</ul>" ;
        $('#allapp').append(contentString);
        var li_high=document.createElement("li");
        li_high.id="high"+entryIndex;
        li_high.innerHTML="<a href='javascript:void(0)' class='unselect' >高压</a>";
        li_high.className="gaoya";
        li_high.style.display="none";
        var li_high_ul=document.createElement("ul");
        li_high_ul.id="high_ul"+entryIndex;
        li_high.appendChild(li_high_ul);
        document.getElementById(entryIndex).appendChild(li_high);

        var li_low=document.createElement("li");
        li_low.id="low"+entryIndex;
        li_low.innerHTML="<a href='javascript:void(0)' class='unselect' >低压</a>";
        li_low.className="diya";
        li_low.style.display="none";
        var li_low_ul=document.createElement("ul");
        li_low_ul.id="low_ul"+entryIndex;
        li_low.appendChild(li_low_ul);
        document.getElementById(entryIndex).appendChild(li_low);

        var url="/showInfo/showSwitchgear/?sid="+sid;
        $.getJSON(url,
            function(data){
                var geardata= data['gears'];
                $.each(geardata,function(gearIndex,obj) {
                    var cid=obj['cid']
                    var type_gui=obj['type_gui']
                    listSwitchgear(cid,type_gui,gearIndex);
                });

            });
        function listSwitchgear(cid,type_gui,gearIndex){
            var li= document.createElement("li");
            li.id=cid;
            li.innerHTML="<a href='javascript:void(0)' class='unselect' >"+cid+"</a>";
            li.className="showLi";
            li.style.display="none";
            li.onclick=function ss(){
                list_cv(1,sid,cid);
                $('#targetPage').attr('src','/CVoltage/info1?sid='+sid+'&dnum='+cid);
                $(this).children("a").attr("class","select");
                $(this).siblings("li").children("a").attr("class","unselect");
                $(this).parent("ul").siblings("ul").children("li").find("a").attr("class","unselect");
            };
            if(type_gui=="高压"){
                document.getElementById("high_ul"+entryIndex).appendChild(li);
            }
            else{
                document.getElementById("low_ul"+entryIndex).appendChild(li);
            }
        }

        $("#"+entryIndex).children("a").click(function(){
            list_cv(1,sid,'');
            $('#targetPage').attr('src','/CVoltage/info1?sid='+sid);
            if($(this).parent().children("li").is(":hidden")){
                $(this).parent().children("li").slideDown("normal");
                $(this).parent().children("li").find("a").attr("class","unselect");
                if($(this).parent("ul").siblings("ul").children("li").is(":visible")){
                    $(this).parent("ul").siblings("ul").find("li").slideUp("1000");
                }
            }else{
                $(this).parent().children("li").slideUp("normal");
            }
        });

        $("#high"+entryIndex).children("a").click(function(){
            if($(this).siblings("ul").children("li").is(":hidden")){
                $(this).siblings("ul").children("li").slideDown("normal");
                $(this).siblings("ul").children("li").find("a").attr("class","unselect");
                $(this).parent().siblings().children("ul").children("li").find("a").attr("class","unselect");
                if($(this).siblings("ul").siblings("ul").children("li").is(":visible")){
                    $(this).siblings("ul").siblings("ul").find("li").slideUp("1000");
                }
                if($(this).parent().siblings().children("ul").children("li").is(":visible")){
                    $(this).parent().siblings().children("ul").find("li").slideUp("1000");

                }
            }else{
                $(this).siblings("ul").children("li").slideUp("normal");
            }

        });
        $("#low"+entryIndex).children("a").click(function(){
            if($(this).siblings("ul").children("li").is(":hidden")){
                $(this).siblings("ul").children("li").slideDown("normal");
                $(this).siblings("ul").children("li").find("a").attr("class","unselect");
                $(this).parent().siblings().children("ul").children("li").find("a").attr("class","unselect");
                if($(this).siblings("ul").siblings("ul").children("li").is(":visible")){
                    $(this).siblings("ul").siblings("ul").find("li").slideUp("1000");
                }
                if($(this).parent().siblings().children("ul").children("li").is(":visible")){
                    $(this).parent().siblings().children("ul").find("li").slideUp("1000");

                }
            }else{
                $(this).siblings("ul").children("li").slideUp("normal");
            }
        });
}
}