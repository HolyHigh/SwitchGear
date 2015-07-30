/**
 * Created with PyCharm.
 * User: joe
 * Date: 14-6-30
 * Time: 下午4:13
 * To change this template use File | Settings | File Templates.
 */
function init(){
    list_cv(1,"-1","-1");
}
function list_cv(page,snum,cnum){
    window.page=page;
    window.snum=snum;
    window.cnum=cnum;
    if(snum=="-1" && cnum=="-1"){
        url="/CVoltage/infoCV/";
    }
    else if(snum !="-1" && cnum=="-1"){
        url="/CVoltage/infoCV/?sid="+snum;
    }
    else if(snum !="-1" && cnum !="-1"){
        url="/CVoltage/infoCV/?sid="+snum+"&dnum="+cnum;
    }
    $.getJSON(url,
        {
            page : page

        }, function(result)  {
            var $div="<div id=count>" +
                "<p>共有：" + count + "条记录</p>" +
                "</div>";
            $('#count').replaceWith($div);
            contentString2="<div id='mpmessage'><table id='cvtable'><thead><thead><tr id='parent'>"+"<td colspan='12' id='thead'>电压电流信息</td>"+"</tr><tr>"+
                "<th>所序号</th><th>柜序号</th><th>A相电压</th><th>B相电压</th><th>C相电压</th><th>A相电流</th><th>B相电流</th><th>C相电流</th>"+
                "<th>A相功率因素</th><th>B相功率因素</th><th>C相功率因素</th><th>更新时间</th>"+
                "</tr></table></div>" ;
            $('#mpmessage').replaceWith(contentString2);

            try{
                var info = result['info'];
                var count = info['count'];
                var numpages = info['numpages'];
                var page = info['page'];
                var sid=info['sid'];
                sid=sid.toString();

                var $divpagenum = "<div id='pagemap' class='pagemap'><p>";
                var prior = parseInt(page) - 1;
                var next = parseInt(page) + 1;

                if(page != 1){
                    $divpagenum = $divpagenum + "<a href='javascript:list_cv(" + prior+","+"&quot;"+sid+"&quot;"+","+"-1"+ ")'>上一页</a>&nbsp;";
                }
                var a, b;
                if(page > 3){
                    a = page - 2;
                }else{
                    a = 1;
                }
                b = a + 4;
                while(b > numpages){
                    b = b -1;
                    if(a > 1){
                        a = a - 1;
                    }
                }

                for(i = a; i <= b; i++){

                    if(i != page){
                        $divpagenum = $divpagenum + "<a href='javascript:list_cv(" + i +","+"&quot;"+sid+"&quot;"+","+"-1"+ ")'>第" + i + "页</a>&nbsp;";
                    }else{
                        $divpagenum = $divpagenum + "第" + i + "页&nbsp;";
                    }
                }
                if(page != numpages){
                    $divpagenum = $divpagenum + "<a href='javascript:list_cv(" + next+","+"&quot;"+sid+"&quot;"+","+"-1"+")'>下一页</a>";
//                    $divpagenum = $divpagenum + "<a href='javascript:list_cv(" + next+","+sid+","+"-1"+")'>下一页</a>";
                }
                $divpagenum = $divpagenum + "</p></div>";
                $('#pagemap').replaceWith($divpagenum);


                var incident = result['incident'];


                $.each(incident,function(entryIndex,entry) {
                    var sid=entry['sid'];
                    var cid = entry['cid'];
                    var Ua = entry['Ua'];
                    var Ub = entry['Ub'];
                    var Uc = entry['Uc'];
                    var Ia = entry['Ia'];
                    var Ib = entry['Ib'];
                    var Ic = entry["Ic"];
                    var PFa=entry["PFa"];
                    var PFb=entry["PFb"];
                    var PFc=entry["PFc"];
                    var update_time=entry["update_time"];
                    list(sid, cid, Ua, Ub, Uc,Ia, Ib,Ic,PFa,PFb,PFc,update_time,entryIndex);
                });

            }catch (e){
                alert(result["error"]);
            }
        });

}
var t= setInterval("list_cv(window.page,window.snum,window.cnum)",5000);

function list(sid, cid, Ua, Ub, Uc,Ia, Ib,Ic,PFa,PFb,PFc,update_time,entryIndex){
    var row=document.createElement("tr");
    row.id=entryIndex;
    row.onclick=function ss(){
        $(this).addClass("select");
        td1=$(this).find('td:eq(0)').text();
        td2=$(this).find('td:eq(1)').text();
        $(document).find("input:radio").attr("checked","true");
        if(document.getElementById("suoid")){
            document.getElementById("suoid").value=td1;
        }
        document.getElementById("suo_id").value=td1;   //这和下面的Jquery方法是有点不一样的。注意区别！
        if(document.getElementById("guiid")){
            document.getElementById("guiid").value=td2;
        }
        document.getElementById("gui_id").value=td2;
    }
    row.onmouseout=function mv(){
        $(this).removeClass("select");
    }

    var cell1=document.createElement("td");
    cell1.innerHTML=sid;
    row.appendChild(cell1);
    var cell2=document.createElement("td");
    cell2.innerHTML=cid;
    row.appendChild(cell2);
    var cell3=document.createElement("td");
    cell3.innerHTML=Ua;
    row.appendChild(cell3);
    var cell4=document.createElement("td");
    cell4.innerHTML=Ub;
    row.appendChild(cell4);
    var cell5=document.createElement("td");
    cell5.innerHTML=Uc;
    row.appendChild(cell5);
    var cell6=document.createElement("td");
    cell6.innerHTML=Ia;
    row.appendChild(cell6);
    var cell7=document.createElement("td");
    cell7.innerHTML=Ib;
    row.appendChild(cell7);
    var cell8=document.createElement("td");
    cell8.innerHTML=Ic;
    row.appendChild(cell8);
    var cell9=document.createElement("td");
    cell9.innerHTML=PFa;
    row.appendChild(cell9);
    var cell10=document.createElement("td");
    cell10.innerHTML=PFb;
    row.appendChild(cell10);
    var cell11=document.createElement("td");
    cell11.innerHTML=PFc;
    row.appendChild(cell11);
    var cell12=document.createElement("td");
    cell12.innerHTML=update_time;
    row.appendChild(cell12);
    document.getElementById("cvtable").appendChild(row);

    $("tr:odd").addClass("odd");
    $("tr:even").addClass("even");

    for(var i= 2;i< 5;i++){
        td_U=$(row).find('td:eq('+i+')').text();
        if(parseFloat(td_U)>220|| parseFloat(td_U)<0){
            $(row).find('td:eq('+i+')').attr("class","cellError");

            var timeStr=update_time;
            var result=timeStr.split(" ");
            var time_str=result[1].split(":");
            var date_str=result[0].split("-");
            var today = new Date();
            var year = today.getFullYear();
            var month = today.getMonth() + 1;
            var day = today.getDate();
            var hour=today.getHours();
            var minute=today.getMinutes();
            var second=today.getSeconds();
            if(Number(date_str[0])==Number(year.toString()) && Number(date_str[1])==Number(month.toString()) && Number(date_str[2])==Number(day.toString()) && Number(time_str[0])==Number(hour.toString()) && Number(time_str[1])==Number(minute.toString())){

                if((Number(second.toString())-Number(time_str[2]))<5){
                    alert(update_time+"\n"+"所序号："+sid+"\n"+"柜序号："+cid+"\n"+"电压有异常发现！");

                }

            }



        }
    }
    for(var j= 5;j< 8;j++){
        td_I=$(row).find('td:eq('+j+')').text();
        if(parseFloat(td_I)>2 || parseFloat(td_I)<0){
            $(row).find('td:eq('+j+')').attr("class","cellError");

            var time_Str=update_time;
            var result_str=time_Str.split(" ");
            var I_time_str=result_str[1].split(":");
            var I_date_str=result_str[0].split("-");
            var I_today = new Date();
            var I_year = I_today.getFullYear();
            var I_month = I_today.getMonth() + 1;
            var I_day = I_today.getDate();
            var I_hour=I_today.getHours();
            var I_minute=I_today.getMinutes();
            var I_second=I_today.getSeconds();
            if(Number(I_date_str[0])==Number(I_year.toString()) && Number(I_date_str[1])==Number(I_month.toString()) && Number(I_date_str[2])==Number(I_day.toString()) && Number(I_time_str[0])==Number(I_hour.toString()) && Number(I_time_str[1])==Number(I_minute.toString())){

                if((Number(I_second.toString())-Number(I_time_str[2]))<5){
                    alert(update_time+"\n"+"所序号："+sid+"\n"+"柜序号："+cid+"\n"+"电流有异常发现！");

                }

            }
        }
    }


}
