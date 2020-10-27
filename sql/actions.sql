--Project
insert into Project(name, description) values ('PogProject', 'kekW');

delete from Project where  name ='PogProject';
delete from Project where id  = 1;

update Project set name = 'OmegaLuLProject', description = '4head' where id = 1;

--DeveloperInfo
insert into DeveloperInfo(name, age, role) values ('Dev1', 20, (select id from DeveloperRole where role = 'Animator'));

delete from DeveloperInfo where name = 'Dev1';
delete from DeveloperInfo where id = 1;

select * from DeveloperInfo;
select DeveloperInfo.id,Dr.role as Designation,name, age from DeveloperInfo join DeveloperRole DR on DR.id = DeveloperInfo.role;

update DeveloperInfo set name = 'Dev4', age = 25, role =1 where  id = 3;
update DeveloperInfo set name = 'Dev4', age = 25, role =(select id from DeveloperRole where role = 'Animator') where  id = 3;

--DeveloperRole
insert into DeveloperRole(role) values ('Animator');

delete from DeveloperRole where  role ='Animator';
delete from DeveloperRole where id = 1;

--Commits
insert into Commits(dev, resource, date, message) values ((select id from Developer where dev_info =
(select id from DeveloperInfo where name = 'Dev1')),(select id from Resource where name = 'Level1'),'2010-10-10','Added saving');

delete from Commits where dev = (select id from Developer where dev_info = (select id from DeveloperInfo where name = 'Dev1'));
delete from Commits where dev = (select id from Developer where dev_info = (select id from DeveloperInfo where name = 'Dev1'))
and resource = (select id from Resource where name = 'Level1') and message ='Added saving';
delete from Commits where id = 1;

select * from Commits where resource = (select id from Resource where name = 'Level1' );
select * from Commits where resource = (select id from Resource where type = (select id from ResourceType where name ='Level'));
select * from  Commits;
select commits.id,dev,Commits.resource,name as name_of_resourse,date,message
from Commits join Resource on Commits.resource = Resource.id;

update Commits set dev = 8, resource =2, date = '2012-10-10', message = 'Add new material' where id = 4;
update  Commits set dev = (select id from Developer where dev_info =(select id from DeveloperInfo where name = 'Dev1')),
resource = (select id from Resource where name = 'Texture1'),date = '2012-10-10', message = 'Add new material' where id = 4;

--ResourceType
insert into ResourceType(name) values ('Mesh');

delete from ResourceType where name = 'Mesh';
delete from ResourceType where id = 1;

select name from ResourceType;


--Dependency
insert into Dependency(requesting_resource, required_resource) values ((select id from Resource where name = 'Level1'),(select id from Resource where name = 'Material1'));

delete from Dependency where requesting_resource =(select id from Resource where name = 'Level1') and required_resource = (select id from Resource where name = 'Material1');
delete from Dependency where id = 1;

select * from Dependency;
select  Dependency.id,Requesting.name,Required.name from Dependency join Resource Requesting on
Dependency.requesting_resource = Requesting.id join Resource Required on Dependency.required_resource=Required.id;

update  Dependency set requesting_resource = 3, required_resource =2 where id = 1;
update Dependency set requesting_resource = (select id from Resource where name = 'Level1'),
required_resource = (select id from Resource where name = 'Texture1') where id = 4;

-- Resource
insert into Resource(name, description, size, checksum, type, data) values ('Level1', '', 12, 'A0EEBC99-9C0B-4EF8-BB6D-6BB9BD380A11',
(select id from ResourceType where name = 'Level'), '1323213212');

delete from Resource where name ='Level1';
delete from Resource where id = 1;

select * from Resource where type = (select id  from ResourceType where name = 'Level');
select * from Resource;

update Resource set name = 'Texture1', description = '', size = 12,
checksum = 'a0eebc99-9c0b-4ef8-bb6d-6bb9bd380a11', type =1, data = 2321231 where id = 2  ;
update Resource set name = 'Texture1',description ='',size =12, checksum='a0eebc99-9c0b-4ef8-bb6d-6bb9bd380a11',
type = (select id from ResourceType where name = 'Texture'), data ='2015-10-02' where id =2;

--Developer
insert into Developer(dev_info, project) values ((select id from DeveloperInfo where name = 'Dev1'), (select id from Project where name = 'PogProject'));

delete from Developer where id =1;
