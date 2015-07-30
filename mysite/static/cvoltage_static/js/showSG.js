/**
 * Created with PyCharm.
 * User: joe
 * Date: 14-6-16
 * Time: 下午2:00
 * To change this template use File | Settings | File Templates.
 */
function init(){
    showSG();
}
function showSG(){
    var sid=$("#sid").val();
    alert(sid);
    var url="/showInfo/showSwitchgear/?sid="+sid;
    $.getJSON(url,
        function(data){
            var geardata= data['gears'];
            $.each(geardata,function(gearIndex,obj) {
                var id=obj['cid'];
                selectstr = "<option value='" + id + "'>" + id + "</option>";
                $('#dnum').append(selectstr);

            });

        });
}