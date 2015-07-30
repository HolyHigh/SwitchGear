/**
 * Created with PyCharm.
 * User: joe
 * Date: 14-7-1
 * Time: 下午3:17
 * To change this template use File | Settings | File Templates.
 */

function init(){
    list_switch(1,"-1",-1);
}
function list_switch(page,snum,cnum){
    window.page=page;
    window.snum=snum;
    window.cnum=cnum;
    if(snum=="-1" && cnum==-1){
        url="/Switch/infoSwitch/";
    }
    else if(snum !="-1" && cnum==-1){
        url="/Switch/infoSwitch/?sid="+snum;
    }
    else if(snum !="-1" && cnum !=-1){
        url="/Switch/infoSwitch/?sid="+snum+"&cnum="+cnum;
    }
    $.getJSON(url,
        {
            page : page

        }, function(result)  {
            var $div="<div id=count>" +
                "<p>共有：" + count + "条记录</p>" +
                "</div>";
            $('#count').replaceWith($div);
            contentString2="<div id='mpmessage'><table id='cvtable'><thead><thead><tr id='parent'>"+"<td colspan='19' id='thead'>开关信息</td>"+"</tr><tr>"+
                "<th>所序号</th><th>柜序号</th><th>断路器</th><th>手工位置</th><th>实验位置</th><th>接地刀</th><th>储能状态</th><th>开关量7</th>"+
                "<th>A相带电状态</th><th>B相带电状态</th><th>C相带电状态</th><th>锁状态</th><th>传感器1</th><th>传感器2</th><th>负载1</th><th>负载2</th>"+
                "<th>回路1</th><th>回路2</th><th>更新时间</th>"+
                "</tr></table></div>" ;
            $('#mpmessage').replaceWith(contentString2);

            try{
                var info = result['info'];
                var count = info['count'];
                var numpages = info['numpages'];
                var page = info['page'];


                var $divpagenum = "<div id='pagemap' class='pagemap'><p>";
                var prior = parseInt(page) - 1;
                var next = parseInt(page) + 1;

                if(page != 1){
                    $divpagenum = $divpagenum + "<a href='javascript:list_switch(" + prior+","+"&quot;"+snum+"&quot;"+","+"-1"+  ")'>上一页</a>&nbsp;";
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
                        $divpagenum = $divpagenum + "<a href='javascript:list_switch(" + i +","+"&quot;"+snum+"&quot;"+","+"-1"+ ")'>第" + i + "页</a>&nbsp;";
                    }else{
                        $divpagenum = $divpagenum + "第" + i + "页&nbsp;";
                    }
                }
                if(page != numpages){
                    $divpagenum = $divpagenum + "<a href='javascript:list_switch(" + next +","+"&quot;"+snum+"&quot;"+","+"-1"+ ")'>下一页</a>";
                }
                $divpagenum = $divpagenum + "</p></div>";
                $('#pagemap').replaceWith($divpagenum);


                var incident = result['incident'];


                $.each(incident,function(entryIndex,entry) {
                    var sid=entry['sid'];
                    var cid = entry['cid'];
                    var S1 = entry['S1'];
                    var S2 = entry['S2'];
                    var S3 = entry['S3'];
                    var S4 = entry['S4'];
                    var S5 = entry['S5'];
                    var S6 = entry["S6"];
                    var S7=entry["S7"];
                    var S8=entry["S8"];
                    var S9=entry["S9"];
                    var S10=entry["S10"];
                    var S11=entry["S11"];
                    var S12=entry["S12"];
                    var S13=entry["S13"];
                    var S14=entry["S14"];
                    var S15=entry["S15"];
                    var S16=entry["S16"];
                    var update_time=entry["update_time"];
                    list(sid, cid, S1, S2, S3,S4, S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16,update_time,entryIndex);
                });

            }catch (e){
                alert(result["error"]);
            }
        });
}

var t= setInterval("list_switch(window.page,window.snum,window.cnum)",5000);

function list(sid, cid, S1, S2, S3,S4, S5,S6,S7,S8,S9,S10,S11,S12,S13,S14,S15,S16,update_time,entryIndex){
    var row=document.createElement("tr");
    row.id=entryIndex;

    var cell1=document.createElement("td");
    cell1.innerHTML=sid;
    row.appendChild(cell1);
    var cell2=document.createElement("td");
    cell2.innerHTML=cid;
    row.appendChild(cell2);
    var cell3=document.createElement("td");
    cell3.innerHTML=S1;
    row.appendChild(cell3);
    var cell4=document.createElement("td");
    cell4.innerHTML=S2;
    row.appendChild(cell4);
    var cell5=document.createElement("td");
    cell5.innerHTML=S3;
    row.appendChild(cell5);
    var cell6=document.createElement("td");
    cell6.innerHTML=S4;
    row.appendChild(cell6);
    var cell7=document.createElement("td");
    cell7.innerHTML=S5;
    row.appendChild(cell7);
    var cell8=document.createElement("td");
    cell8.innerHTML=S6;
    row.appendChild(cell8);
    var cell9=document.createElement("td");
    cell9.innerHTML=S7;
    row.appendChild(cell9);
    var cell10=document.createElement("td");
    cell10.innerHTML=S8;
    row.appendChild(cell10);
    var cell11=document.createElement("td");
    cell11.innerHTML=S9;
    row.appendChild(cell11);
    var cell12=document.createElement("td");
    cell12.innerHTML=S10;
    row.appendChild(cell12);
    var cell13=document.createElement("td");
    cell13.innerHTML=S11;
    row.appendChild(cell13);
    var cell14=document.createElement("td");
    cell14.innerHTML=S12;
    row.appendChild(cell14);
    var cell15=document.createElement("td");
    cell15.innerHTML=S13;
    row.appendChild(cell15);
    var cell16=document.createElement("td");
    cell16.innerHTML=S14;
    row.appendChild(cell16);
    var cell17=document.createElement("td");
    cell17.innerHTML=S15;
    row.appendChild(cell17);
    var cell18=document.createElement("td");
    cell18.innerHTML=S16;
    row.appendChild(cell18);
    var cell19=document.createElement("td");
    cell19.innerHTML=update_time;
    row.appendChild(cell19);
    document.getElementById("cvtable").appendChild(row);

    $("tr:odd").addClass("odd");
    $("tr:even").addClass("even");

    for(i=2;i<=17;i++){
        td=$(row).find('td:eq('+i+')').text();
        if(td=="true"){
            $(row).find('td:eq('+i+')').text("开");
        }else{
            $(row).find('td:eq('+i+')').text("关");
        }

    }

}